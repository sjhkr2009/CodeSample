using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Newtonsoft.Json;

public static class SaveHelper {
    private static string RootPath { get; } = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
    public enum Type {
        TextResult,
        JsonData,
        TextFullLog,
        CsvData
    }

    public static string GetPath(Type type) {
        string fileName = type switch {
            Type.TextResult => "AnalysisOutput_result.txt",
            Type.JsonData => "AnalysisOutput_data.json",
            Type.TextFullLog => "AnalysisOutput_log.txt",
            Type.CsvData => "AnalysisOutput_data.csv",
            _ => string.Empty
        };
        return Path.Combine(RootPath, fileName);
    }
    
    public static void SaveToJson(List<Company> companies) {
        var json = JsonConvert.SerializeObject(companies, Formatting.Indented);
        File.WriteAllText(GetPath(Type.JsonData), json);
    }
    public static void SaveLog(StringBuilder log) {
        File.WriteAllText(GetPath(Type.TextFullLog), log.ToString());
    }
    public static void SaveToTextFile(List<Company> companies) {
        StringBuilder output = new StringBuilder();
        companies.SortByCompanyPoint();

        for (int i = 0; i < companies.Count; i++) {
            var company = companies[i];
            output.AppendLine($"[{i + 1}위] {company.CompanyName} ({company.Code:000000}) : " +
                              $"{(company.RecommendPoint - company.WarningPoint)}점 " +
                              $"({company.RecommendPoint} - {company.WarningPoint})" +
                              (string.IsNullOrEmpty(company.Section) ? string.Empty : $" - {company.Section}"));
        }

        Dictionary<string, List<Company>> sections = new Dictionary<string, List<Company>>();
        foreach (var company in companies) {
            if (string.IsNullOrEmpty(company.Section)) continue;
			
            if (sections.TryGetValue(company.Section, out var list)) list.Add(company);
            else sections.Add(company.Section, new List<Company>() {company});
        }

        foreach (var section in sections) {
            if (section.Value.Count < 10) continue;
            output.AppendLine("\n---------------------------------------------------");
            output.AppendLine($"업종별 순위 - {section.Key}");
            for (int i = 0; i < section.Value.Count; i++) {
                var company = section.Value[i];
                output.AppendLine($"[{i + 1}위] {company.CompanyName} ({company.Code:000000}) : " +
                                  $"{(company.RecommendPoint - company.WarningPoint)}점 " +
                                  $"({company.RecommendPoint} - {company.WarningPoint})");
            }
        }
        
        File.WriteAllText(GetPath(Type.TextResult), output.ToString());
    }

    public static void SaveToCsv(List<Company> companies) {
        StringBuilder csv = new StringBuilder();
        csv.AppendLine(GetCompanyHeader());
        foreach (var company in companies) {
            csv.AppendLine(company.ToCsvStream());
        }
        File.WriteAllText(GetPath(Type.CsvData), csv.ToString());
    }

    private static string ToCsvStream(this Company company) {
        return $"{company.CompanyName}," +
               $"{company.Code:000000}," +
               $"{company.Section.Replace(',', '/')}," +
               $"{company.MarketCap}," +
               $"{company.TotalStockCount}," +
               $"{company.CurrentPrice}," +
               $"{company.Per}," +
               $"{company.ExpectedPer}," +
               $"{company.Pbr}," +
               $"{company.DividendRate}," +
               $"{company.SimilarCompanyPer}," +
               $"{company.WarningPoint}," +
               $"{company.RecommendPoint}," +
               $"{company.YearPerformances[0].ToCsvStream()}," +
               $"{company.YearPerformances[1].ToCsvStream()}," +
               $"{company.YearPerformances[2].ToCsvStream()}," +
               $"{company.YearPerformances[3].ToCsvStream()}," +
               $"{company.QuarterPerformances[0].ToCsvStream()}," +
               $"{company.QuarterPerformances[1].ToCsvStream()}," +
               $"{company.QuarterPerformances[2].ToCsvStream()}," +
               $"{company.QuarterPerformances[3].ToCsvStream()}," +
               $"{company.QuarterPerformances[4].ToCsvStream()}";
    }
    
    private static string ToCsvStream(this Company.Performance performance) {
        return $"{performance.SalesRevenue}," +
               $"{performance.GrossProfit}," +
               $"{performance.NetProfit}," +
               $"{performance.Roe}," +
               $"{performance.DebtRatio}," +
               $"{performance.QuickRatio}," +
               $"{performance.ReserveRation}," +
               $"{performance.Eps}," +
               $"{performance.Per}," +
               $"{performance.DividendRate}," +
               $"{performance.DividendPayoutRatio}";
    }

    
    private static string GetCompanyHeader() {
        return "회사명," +
               "종목코드," +
               "업종," +
               "시가총액," +
               "상장주식수," +
               "현재가격," +
               "PER," +
               "예상PER," +
               "PBR," +
               "배당수익률," +
               "동일업종 PER," +
               "감점합계," +
               "가점합계," +
               $"{GetPerformanceHeader("3년 전")}," +
               $"{GetPerformanceHeader("2년 전")}," +
               $"{GetPerformanceHeader("1년 전")}," +
               $"{GetPerformanceHeader("올해 예상")}," +
               $"{GetPerformanceHeader("4분기 전")}," +
               $"{GetPerformanceHeader("3분기 전")}," +
               $"{GetPerformanceHeader("2분기 전")}," +
               $"{GetPerformanceHeader("1분기 전")}," +
               $"{GetPerformanceHeader("다음분기 예상")}";
        
        string GetPerformanceHeader(string postfix) {
            return $"매출({postfix})," +
                   $"영업이익({postfix})," +
                   $"당기순이익({postfix})," +
                   $"ROE({postfix})," + 
                   $"부채비율({postfix})," +
                   $"당좌비율({postfix})," + 
                   $"자본유보율({postfix})," + 
                   $"EPS({postfix})," + 
                   $"PER({postfix})," + 
                   $"시가배당률({postfix})," + 
                   $"배당성향({postfix})";
        }
    }
}
