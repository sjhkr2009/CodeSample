#include "PCH.h"
#include "Game.h"

#include <sstream>
#include <fstream>

// 플랫폼(윈도우, XBOX 등)마다 진입점과 종료 방식이 다르므로, 종료 기능은 extern 함수로 만들고 WinMain.cpp에 별도로 만들기로 한다.
extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
}

// 키보드 및 마우스 클래스, 장치와 디바이스 컨텍스트, 해상도 종속 리소스들을 생성합니다.
void Game::Initialize(HWND hwnd, int width, int height)
{
	// 키보드/마우스 클래스 생성 (싱글톤이므로 유니크 포인터 사용)
	// 싱글톤도 Instance() 최초 호출로 생성하는 게 아니라, RAII 원칙에 따라 직접 생성하여 생성 시점을 명확하게 한다.
	m_keyboard = std::make_unique<Keyboard>();

	// DirectX의 마우스 클래스는 윈도우 핸들을 지정해야 작동한다.
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(hwnd);

	// 기기 및 기기 종속 리소스 생성
	m_deviceResources->SetWindow(hwnd, width, height);
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	// 해상도 종속 리소스 생성
	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	//LoadSpriteSheetsFromJson();
}

#pragma region Frame Update

/// <summary>
/// 메시지 루프에서 매 틱마다 호출됩니다. StepTimer를 통해 게임 로직을 실행하고(Update) 화면을 그리는 동작(Render)을 실행합니다.
/// Render()는 백 버퍼를 렌더링한 후 Device Resources에서 수직 동기화 여부를 확인한 후 지연시간을 설정하여 Present()를 호출하므로 매 틱마다 수행됩니다.
/// </summary>
void Game::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		}
	);

	Render();
}

// StepTimer를 통해 매 프레임마다 호출됩니다. 키보드의 상태를 갱신하고 특정 키 입력에 대한 동작을 호출합니다.
void Game::Update(DX::StepTimer const& timer)
{
	auto kb = m_keyboard->GetState();

	// kb.KeyCode : KeyCode에 해당하는 키가 눌렸다면 true를 반환한다.
	if (kb.Escape)
	{
		ExitGame();
	}

	if (kb.D1)
	{
		SetAnimation(L"Move");
	}
	if (kb.D2)
	{
		SetAnimation(L"Die");
	}

	m_timeToNextFrame -= timer.GetElapsedSeconds();
	addAnimCooldown -= timer.GetElapsedSeconds();
	if (m_timeToNextFrame < 0.0)
	{
		if (!m_animQueue.empty())
		{
			Frame frame = m_animQueue.front();
			m_animQueue.pop();

			m_rect = GetRectByFilename(frame.spriteName);
			m_timeToNextFrame = frame.duration;
		}
		else
		{
			m_rect = { 0, 0, 0, 0 };
		}
	}
	if (addAnimCooldown < 0.0)
	{
		canAddAnim = true;
	}
}

#pragma endregion

#pragma region Frame Render

/// <summary>
/// 화면을 그리는 동작을 수행하며 매 틱마다 호출됩니다. 화면을 클리어하고 백 버퍼에 그림을 그린 후, 스왑 체인의 Present()를 호출합니다.
/// 수직 동기화 여부를 확인하는 작업은 DeviceResources::Present()에서 수행합니다.
/// </summary>
void Game::Render()
{
	if (m_timer.GetTotalFrameCount() == 0)
	{
		// 한 번도 업데이트되지 않은 상태에서는 그리지 않는다.
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");

	// TODO: 그리기 코드
	//-------------------------------------------------------------
	// SpriteBatch는 Begin ~ End 사이의 그리기 작업을 일괄적으로 수행한다.

	m_spriteBatch->Begin(
		SpriteSortMode_Deferred,
		m_commonStates->NonPremultiplied()
	);

	m_spriteBatch->Draw(m_texture.Get(), XMFLOAT2(50.f, 50.f), &m_rect);

	m_spriteBatch->End();
	//-------------------------------------------------------------

	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
}

// 화면을 그리기 전에 백 버퍼에 연결된 View를 초기화하고, 렌더 타겟과 뷰포트를 세팅합니다.
void Game::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL), 1.0f, 0);

	// 만약 렌더 타겟이나 뷰포트가 여러 개일 경우(게임에 거울이 존재하는 등), 렌더 타겟 및 뷰포트 지정도 여러 번 해 준다.
	// 여기서는 클리어 대상인 백 버퍼로 세팅하고, 렌더 타겟을 별도로 만들었다면 Render()에서 세팅해야 한다.

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

#pragma endregion

#pragma region Message Handler

// WM_ACTIVATEAPP 메시지에 따라 게임 활성화 시 작동합니다.
void Game::OnActivated()
{
}

// WM_ACTIVATEAPP 메시지에 따라 게임 비활성화 시 작동합니다.
void Game::OnDeactivated()
{
}

// 일시정지 시 호출됩니다. 창 최소화 및 절전 모드 진입 시 자동으로 호출됩니다.
void Game::OnSuspending()
{
}

// 일시정지 해제 시 호출됩니다. 창 최소화 해제 및 절전 모드 해제 시 자동으로 호출됩니다.
void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
}

// 프로그램이 다른 디스플레이로 이동했을 때 작동합니다. DeviceResources에 현재 창 크기를 넘겨서 색상 공간만 업데이트합니다.
void Game::OnWindowMoved()
{
	auto rct = m_deviceResources->GetOutputSize();
	m_deviceResources->OnWindowSizeChanged(rct.right, rct.bottom);
}

/// <summary>
/// 프로그램의 창 크기가 변경되었을 때 외부에서 호출됩니다. DeviceResources에 변경된 창 크기를 넘겨서 색상 공간을 업데이트합니다.
/// 실제 창 크기가 변경되었다면 DeviceResources에서 해상도 종속 리소스가 재생성되며, 이후 이 게임에서 사용되는 리소스들도 재생성합니다.
/// </summary>
void Game::OnWindowSizeChanged(int width, int height)
{
	if (m_deviceResources->OnWindowSizeChanged(width, height))
	{
		CreateWindowSizeDependentResources();
	}
}

// 상수 대신 사용하는 함수. 윈도우 크기의 디폴트값을 반환합니다.
void Game::GetDefaultSIze(int& width, int& height) const noexcept
{
	width = 800;
	height = 600;
}

#pragma endregion

#pragma region Direct3D Resources

// 이 게임에서 사용되는 장치 종속 리소스를 생성합니다.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_commonStates = std::make_unique<CommonStates>(device);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	CreateWICTextureFromFile(device, L"Assets/spriteSheet.png", nullptr, &m_texture);
	animationDoc = LoadJson(L"animation.json");
	spriteSheetDoc = LoadJson(L"Assets/spriteSheet.json");
}

// 이 게임에서 사용되는 해상도 종속 리소스를 생성합니다.
void Game::CreateWindowSizeDependentResources()
{

}
/*
void Game::LoadSpriteSheetsFromJson()
{
	m_rects.clear();

	rapidjson::GenericDocument<rapidjson::UTF16<>> doc = LoadJson(L"Asstes/spriteSheet.json");

	auto frames = doc[L"frames"].GetArray();
	RECT rect{};
	for (auto& e : frames)
	{
		auto obj = e[L"frame"].GetObject();

		rect.left = obj[L"x"].GetInt();
		rect.top = obj[L"y"].GetInt();
		rect.right = rect.left + obj[L"w"].GetInt();
		rect.bottom = rect.top + obj[L"h"].GetInt();

		m_rects.push_back(rect);
	}
}
*/

void Game::SetAnimation(const wchar_t* name)
{
	if (!canAddAnim)
		return;
	
	auto animations = animationDoc[L"animation"].GetArray();

	double timeToNextFrame{};
	bool isLoop;

	for (auto& e : animations)
	{
		if (!wcscmp(e[L"name"].GetString(), name))
		{
			auto frameInfo = e[L"frameInfo"].GetArray();
			Frame newFrame{};
			for (auto& frame : frameInfo)
			{
				newFrame.duration = frame[L"duration"].GetDouble();
				newFrame.spriteName = frame[L"frameName"].GetString();

				m_animQueue.push(newFrame);
			}

			break;
		}

		// TODO: RECT 벡터는 필요없음. spriteSheet도 읽어서 멤버로 들고 있을 것.
		//		 Update(현재 그릴 영역의 rect와 지속시간에 대한 처리)와 Render(그리기) 동작을 구분할 것.
		//		 멤버로 그릴 영역의 RECT를 선언해두고, Update에서 그릴 영역을 저장하고 Render에서 해당 정보에 따라 그릴 것.
	}

	canAddAnim = false;
	addAnimCooldown = 0.5;
}

RECT Game::GetRectByFilename(const wchar_t* spriteName)
{
	auto frames = spriteSheetDoc[L"frames"].GetArray();
	RECT rect{};

	for (auto& e : frames)
	{
		if (!wcscmp(e[L"filename"].GetString(), spriteName))
		{
			auto obj = e[L"frame"].GetObject();

			rect.left = obj[L"x"].GetInt();
			rect.top = obj[L"y"].GetInt();
			rect.right = rect.left + obj[L"w"].GetInt();
			rect.bottom = rect.top + obj[L"h"].GetInt();

			break;
		}
	}

	return rect;
}

void Game::GetFrameInfo(const wchar_t* animName, int index, double& duration, RECT& rect)
{

}

rapidjson::GenericDocument<rapidjson::UTF16<>> Game::LoadJson(const wchar_t* filename)
{
	std::wifstream file(filename, std::wifstream::binary);
	std::wstringstream stream;

	stream << file.rdbuf();
	file.close();

	// rapidjson::Document는, 내부적으로 GenericDocument<rapidjson::UTF8<>> 로 정의되어 있다. 다른 인코딩 방식을 쓰려면 이렇게 쓴다.
	rapidjson::GenericDocument<rapidjson::UTF16<>> doc;
	doc.Parse(stream.str().c_str());

	return doc;
}

// 장치 손실 시 이 게임에서 처리할 동작입니다. 
void Game::OnDeviceLost()
{

	m_spriteBatch.reset();
	m_commonStates.reset();
}

// 장치 복구 시 이 게임에서 처리할 동작입니다. 리소스를 다시 생성합니다.
void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma endregion


