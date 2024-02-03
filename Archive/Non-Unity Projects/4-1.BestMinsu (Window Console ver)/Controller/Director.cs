using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;	
using OpenQA.Selenium.Chrome;	
using OpenQA.Selenium;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Cysharp.Threading.Tasks;
using DevNet;
using Newtonsoft.Json;

// .NET 6.0 버전이 필요합니다
public static class Director {
	private static StringBuilder log = new StringBuilder();
	public static Control logDrawer;

	private static readonly List<Company> Companies = new List<Company>();
	private static readonly LinkedList<IWebDriver> RunningDrivers = new LinkedList<IWebDriver>();

	private static List<Company> CachedCompanies = new List<Company>();
	private static string CachedLog = string.Empty;

	public enum RunMode {
		AnalysisFromWeb,
		AnalysisFromJson
	}
	public static RunMode CurrentRunMode = RunMode.AnalysisFromWeb;

	public static void Initialize(Control logger) {
		logDrawer = logger;
		AnalysisHelper.LogDrawer = logDrawer;
	}

	private static int RunningCount { get; set; } = 0;
	private static bool RequestCancel { get; set; } = false;

	private static int progressMax = 0;
	private static int failCount = 0;
	private static readonly Queue<int> targetQueue = new Queue<int>();
	
	public static void Run(Action onComplete = null) {
		if (RunningCount > 0) return;
		
		OnStart();

		if (!QuestionHelper.CheckQuestion()) {
			onComplete?.Invoke();
			return;
		}

		Stopwatch stopwatch = Stopwatch.StartNew();

		if (!CheckDriver()) {
			onComplete?.Invoke();
			return;
		}

		Thread task = new Thread(() => {});
		Thread waitOnComplete = new Thread(() => WaitRun(() => {
				onComplete?.Invoke();
				
				AnalysisHelper.AddSectionOnLog(Companies);
				log.AppendLine($"분석 소요시간: {(double)stopwatch.ElapsedMilliseconds / 1000}s");
				ExtractLogToFile();

				AnalysisHelper.ShowMessage($"분석이 완료되었습니다.\n" +
				                           $"분석 소요시간: {(double)stopwatch.ElapsedMilliseconds / 1000:0.0}초");

				stopwatch.Stop();
			})
		);

		if (CurrentRunMode == RunMode.AnalysisFromWeb) {
			task = new Thread(() => AnalysisAllFromWebMultiTask(1));
		} if (CurrentRunMode == RunMode.AnalysisFromJson) {
			task = new Thread(() => AnalysisAllFromJson());
		}
		
		task.Start();
		waitOnComplete.Start();
	}

	static bool CheckDriver() {
		try {
			AnalysisHelper.ShowMessage("크롬 드라이버가 최신인지 확인 중...");
			var path = Path.Combine(Directory.GetCurrentDirectory(), "chromedriver.exe");
			File.Delete(path);
			//new ChromeDriverUpdater.ChromeDriverUpdater().Update(path);
			ChromeDriverManager.InstallLatest();
			return true;
		} catch (Exception e) {
			AnalysisHelper.ShowMessage("에러가 발생했습니다.\n" +
			                           $"[{e.GetType().Name}] {e.Message}");
			return false;
		}
	}

	static async Task WaitRun(Action onComplete) {
		while (RunningCount == 0) {
			await UniTask.Yield();
		}
		while (RunningCount > 0) {
			await UniTask.Yield();
			
			if (RequestCancel) {
				await Task.Delay(1000);
				AnalysisHelper.ShowMessage("취소 중...");
				await Task.Delay(3000);

				CachedCompanies = Companies.ToList();
				CachedLog = log.ToString();
				break;
			}
		}

		RequestCancel = false;
		onComplete?.Invoke();
	}

	public static async void Stop() {
		RequestCancel = true;
		AnalysisHelper.ShowMessage("중단 중...");
		await Task.Delay(3000);
		ClearDrivers();
	}

	static void ClearDrivers() {
		if (RunningDrivers.Count == 0) return;
		
		if (RunningDrivers.Count > 0) {
			AnalysisHelper.ShowMessage($"{RunningDrivers.Count}개의 작동중인 드라이버를 정리하는 중입니다...");
		}
		foreach (var driver in RunningDrivers) {
			driver?.Quit();
			driver?.Dispose();
		}
		RunningDrivers.Clear();
	}

	static void OnStart() {
		RequestCancel = false;
		log.Clear();
		AnalysisHelper.Initialize(log);
	}
	
	/// <summary>
	/// json 파일의 모든 기업을 Companies 배열에 Deserialize하고, 평가를 초기화한 후 분석하여 점수를 재산정합니다. 
	/// </summary>
	static async Task AnalysisAllFromJson() {
		RunningCount++;
		Stopwatch s = Stopwatch.StartNew();
		long prevElapsedTime = 0;
		
		var json = File.ReadAllText(SaveHelper.GetPath(SaveHelper.Type.JsonData));
		List<Company> targets = JsonConvert.DeserializeObject<List<Company>>(json);
		
		Companies.Clear();
		progressMax = targets.Count;
		
		foreach (var company in targets) {
			AnalysisHelper.ShowMessage($"분석 중... ({Companies.Count + 1} / {progressMax})\n");
			Companies.Add(company.AnalysisAll());
			log.AppendLine();

			if (s.ElapsedMilliseconds - prevElapsedTime > 500) {
				if (RequestCancel) {
					break;
				}
				
				await Task.Yield();
				prevElapsedTime = s.ElapsedMilliseconds;
			}
		}

		RunningCount--;
	}

	static async Task AnalysisAllFromWebMultiTask(int windowCount, int[] targets = null) {
		AnalysisHelper.ShowMessage("크롬을 백그라운드에서 실행하는 중입니다.\n잠시만 기다려주세요.");
		
		targets ??= TargetData.AllListedCompanies;
		InitBeforeEnterWeb(targets);

		UniTask[] tasks = new UniTask[windowCount];
		for (int i = 0; i < windowCount; i++) {
			tasks[i] = AnalysisFromWeb();
		}

		await UniTask.WhenAll(tasks);
		
		ClearDrivers();
	}
	
	/// <summary>
	/// 배열 내 모든 종목을 네이버 금융에서 찾아 분석하고 Companies 리스트에 저장합니다.
	/// </summary>
	static async Task AnalysisAllFromWeb(int[] targets = null) {
		targets ??= TargetData.AllListedCompanies;
		InitBeforeEnterWeb(targets);
		
		await AnalysisFromWeb();
	}

	static void InitBeforeEnterWeb(int[] targets) {
		progressMax = targets.Length;
		Companies.Clear();
		Companies.AddRange(CachedCompanies);
		log.AppendLine(CachedLog);
		
		targetQueue.Clear();
		foreach (var target in targets) {
			if (Companies.Any(c => c.Code == target)) continue;
			targetQueue.Enqueue(target);
		}
	}

	static async UniTask AnalysisFromWeb() {
		// 명령 프롬프트 창을 숨긴다.
		var chromeDriverService = ChromeDriverService.CreateDefaultService();
		chromeDriverService.HideCommandPromptWindow = true;
		
		// 크롬 창을 숨기고 백그라운드에서 실행한다.
		var option = new ChromeOptions();
		option.AddArgument("--headless");
		
		if (RequestCancel) return;
		
		IWebDriver driver = new ChromeDriver(chromeDriverService, option);
		RunningDrivers.AddLast(driver);
		RunningCount++;

		// 대기 설정. (find로 객체를 찾을 때까지 검색이 되지 않으면 대기하는 시간, 초단위)
		driver.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(1);

		try {
			while (targetQueue.Count > 0 && !RequestCancel) {
				var target = targetQueue.Dequeue();
				if (Companies.Any(c => c.Code == target)) continue;

				try {
					driver.Manage().Timeouts().ImplicitWait = TimeSpan.FromSeconds(1);
					var company = Company.CreateFromWeb(driver, target);
					AnalysisHelper.ShowMessage($"분석 중... ({Companies.Count + failCount + 1} / {progressMax})\n");
					AnalysisHelper.AddMessage($"{company.CompanyName}({company.Code:000000}) - {company.Section}\n");
					Companies.Add(company.AnalysisAll());
				}
				catch (Exception e) {
					log.AppendLine($"{target}을 분석하던 중 오류가 발생했습니다.");
					log.AppendLine($"[{e.GetType().Name}] {e.Message} / {e.StackTrace}");
					failCount++;
				}

				log.AppendLine();
				await UniTask.Yield();
			}
		}
		catch (Exception) { }
		finally {
			RunningCount--;
			RunningDrivers.Remove(driver);
			driver.Quit();
		}
	}
	
	/// <summary>
	/// Companies의 종목들을 json 파일로 저장하고, 높은 점수 순으로 정렬한 output 및 로그를 저장합니다.
	/// </summary>
	static void ExtractLogToFile() {
		SaveHelper.SaveLog(log);
		SaveHelper.SaveToJson(Companies);
		SaveHelper.SaveToTextFile(Companies);
		SaveHelper.SaveToCsv(Companies);
	}
}
