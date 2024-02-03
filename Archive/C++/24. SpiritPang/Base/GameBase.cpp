#include "PCH.h"

// �÷���(������, XBOX ��)���� �������� ���� ����� �ٸ��Ƿ�, ���� ����� extern �Լ��� ����� WinMain.cpp�� ������ ������ �Ѵ�.
extern void ExitGame() noexcept;

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace DX;

using Microsoft::WRL::ComPtr;

GameBase::GameBase() noexcept(false)
{
	m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
}

GameBase::~GameBase()
{
	TextureManager::Instance().ReleaseAll();
	JsonManager::Instance().ReleaseAll();
}

// Ű���� �� ���콺 Ŭ����, ��ġ�� ����̽� ���ؽ�Ʈ, �ػ� ���� ���ҽ����� �����մϴ�.
void GameBase::Initialize(HWND hwnd, int width, int height)
{
	InputManager::Instance().Initialize(hwnd);

	// Ű����/���콺 Ŭ���� ���� (�̱����̹Ƿ� ����ũ ������ ���)
	// �̱��浵 Instance() ���� ȣ��� �����ϴ� �� �ƴ϶�, RAII ��Ģ�� ���� ���� �����Ͽ� ���� ������ ��Ȯ�ϰ� �Ѵ�.
	//m_keyboard = std::make_unique<Keyboard>();

	// DirectX�� ���콺 Ŭ������ ������ �ڵ��� �����ؾ� �۵��Ѵ�.
	//m_mouse = std::make_unique<Mouse>();
	//m_mouse->SetWindow(hwnd);

	// ��� �� ��� ���� ���ҽ� ����
	m_deviceResources->SetWindow(hwnd, width, height);
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	// �ػ� ���� ���ҽ� ����
	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	TextureManager::Instance().Initialize(m_deviceResources.get());
}

#pragma region Frame Update

/// <summary>
/// �޽��� �������� �� ƽ���� ȣ��˴ϴ�. StepTimer�� ���� ���� ������ �����ϰ�(Update) ȭ���� �׸��� ����(Render)�� �����մϴ�.
/// Render()�� �� ���۸� �������� �� Device Resources���� ���� ����ȭ ���θ� Ȯ���� �� �����ð��� �����Ͽ� Present()�� ȣ���ϹǷ� �� ƽ���� ����˴ϴ�.
/// </summary>
void GameBase::Tick()
{
	m_timer.Tick([&]()
		{
			Update(m_timer);
		}
	);

	Render();
}

// StepTimer�� ���� �� �����Ӹ��� ȣ��˴ϴ�. Ű������ ���¸� �����ϰ� Ư�� Ű �Է¿� ���� ������ ȣ���մϴ�.
void GameBase::Update(StepTimer const& timer)
{
	InputManager::Instance().Update();

	if (InputManager::Instance().IsKeyDown(Keyboard::Escape))
	{
		ExitGame();
	}

	ActorManager::Instance().Update(timer.GetElapsedSeconds());
}

#pragma endregion

#pragma region Frame Render

/// <summary>
/// ȭ���� �׸��� ������ �����ϸ� �� ƽ���� ȣ��˴ϴ�. ȭ���� Ŭ�����ϰ� �� ���ۿ� �׸��� �׸� ��, ���� ü���� Present()�� ȣ���մϴ�.
/// ���� ����ȭ ���θ� Ȯ���ϴ� �۾��� DeviceResources::Present()���� �����մϴ�.
/// </summary>
void GameBase::Render()
{
	if (m_timer.GetTotalFrameCount() == 0)
	{
		// �� ���� ������Ʈ���� ���� ���¿����� �׸��� �ʴ´�.
		return;
	}

	Clear();

	m_deviceResources->PIXBeginEvent(L"Render");

	// TODO: �׸��� �ڵ�
	//-------------------------------------------------------------
	// SpriteBatch�� Begin ~ End ������ �׸��� �۾��� �ϰ������� �����Ѵ�.

	m_spriteBatch->Begin(
		SpriteSortMode_Deferred,
		m_commonStates->NonPremultiplied()
	);

	ActorManager::Instance().Render(m_spriteBatch.get());

	m_spriteBatch->End();
	//-------------------------------------------------------------

	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
}

// ȭ���� �׸��� ���� �� ���ۿ� ����� View�� �ʱ�ȭ�ϰ�, ���� Ÿ�ٰ� ����Ʈ�� �����մϴ�.
void GameBase::Clear()
{
	m_deviceResources->PIXBeginEvent(L"Clear");

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
	context->ClearDepthStencilView(depthStencil, (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL), 1.0f, 0);

	// ���� ���� Ÿ���̳� ����Ʈ�� ���� ���� ���(���ӿ� �ſ��� �����ϴ� ��), ���� Ÿ�� �� ����Ʈ ������ ���� �� �� �ش�.
	// ���⼭�� Ŭ���� ����� �� ���۷� �����ϰ�, ���� Ÿ���� ������ ������ٸ� Render()���� �����ؾ� �Ѵ�.

	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	m_deviceResources->PIXEndEvent();
}

#pragma endregion

#pragma region Message Handler

// WM_ACTIVATEAPP �޽����� ���� ���� Ȱ��ȭ �� �۵��մϴ�.
void GameBase::OnActivated()
{
	InputManager::Instance().Reset();
}

// WM_ACTIVATEAPP �޽����� ���� ���� ��Ȱ��ȭ �� �۵��մϴ�.
void GameBase::OnDeactivated()
{
}

// �Ͻ����� �� ȣ��˴ϴ�. â �ּ�ȭ �� ���� ��� ���� �� �ڵ����� ȣ��˴ϴ�.
void GameBase::OnSuspending()
{
}

// �Ͻ����� ���� �� ȣ��˴ϴ�. â �ּ�ȭ ���� �� ���� ��� ���� �� �ڵ����� ȣ��˴ϴ�.
void GameBase::OnResuming()
{
	InputManager::Instance().Reset();

	m_timer.ResetElapsedTime();
}

// ���α׷��� �ٸ� ���÷��̷� �̵����� �� �۵��մϴ�. DeviceResources�� ���� â ũ�⸦ �Ѱܼ� ���� ������ ������Ʈ�մϴ�.
void GameBase::OnWindowMoved()
{
	auto rct = m_deviceResources->GetOutputSize();
	m_deviceResources->OnWindowSizeChanged(rct.right, rct.bottom);
}

/// <summary>
/// ���α׷��� â ũ�Ⱑ ����Ǿ��� �� �ܺο��� ȣ��˴ϴ�. DeviceResources�� ����� â ũ�⸦ �Ѱܼ� ���� ������ ������Ʈ�մϴ�.
/// ���� â ũ�Ⱑ ����Ǿ��ٸ� DeviceResources���� �ػ� ���� ���ҽ��� ������Ǹ�, ���� �� ���ӿ��� ���Ǵ� ���ҽ��鵵 ������մϴ�.
/// </summary>
void GameBase::OnWindowSizeChanged(int width, int height)
{
	if (m_deviceResources->OnWindowSizeChanged(width, height))
	{
		CreateWindowSizeDependentResources();
	}
}

// ��� ��� ����ϴ� �Լ�. ������ ũ���� ����Ʈ���� ��ȯ�մϴ�.
void GameBase::GetDefaultSIze(int& width, int& height) const noexcept
{
	width = screenWidth;
	height = screenHeight;
}

#pragma endregion

#pragma region Direct3D Resources

// �� ���ӿ��� ���Ǵ� ��ġ ���� ���ҽ��� �����մϴ�.
void GameBase::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_commonStates = std::make_unique<CommonStates>(device);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

}

// �� ���ӿ��� ���Ǵ� �ػ� ���� ���ҽ��� �����մϴ�.
void GameBase::CreateWindowSizeDependentResources()
{

}


// ��ġ �ս� �� �� ���ӿ��� ó���� �����Դϴ�. 
void GameBase::OnDeviceLost()
{
	m_spriteBatch.reset();
	m_commonStates.reset();

	TextureManager::Instance().OnDeviceLost();
	ActorManager::Instance().OnDeviceLost();
}

// ��ġ ���� �� �� ���ӿ��� ó���� �����Դϴ�. ���ҽ��� �ٽ� �����մϴ�.
void GameBase::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	TextureManager::Instance().OnDeviceRestored();
	ActorManager::Instance().OnDeviceRestored();
}

#pragma endregion


