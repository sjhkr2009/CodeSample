#include "PCH.h"
#include "Game.h"

// �÷���(������, XBOX ��)���� �������� ���� ����� �ٸ��Ƿ�, ���� ����� extern �Լ��� ����� WinMain.cpp�� ������ ������ �Ѵ�.
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

// Ű���� �� ���콺 Ŭ����, ��ġ�� ����̽� ���ؽ�Ʈ, �ػ� ���� ���ҽ����� �����մϴ�.
void Game::Initialize(HWND hwnd, int width, int height)
{
	// Ű����/���콺 Ŭ���� ���� (�̱����̹Ƿ� ����ũ ������ ���)
	// �̱��浵 Instance() ���� ȣ��� �����ϴ� �� �ƴ϶�, RAII ��Ģ�� ���� ���� �����Ͽ� ���� ������ ��Ȯ�ϰ� �Ѵ�.
	m_keyboard = std::make_unique<Keyboard>();

	// DirectX�� ���콺 Ŭ������ ������ �ڵ��� �����ؾ� �۵��Ѵ�.
	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(hwnd);

	// ��� �� ��� ���� ���ҽ� ����
	m_deviceResources->SetWindow(hwnd, width, height);
	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	// �ػ� ���� ���ҽ� ����
	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma region Frame Update

/// <summary>
/// �޽��� �������� �� ƽ���� ȣ��˴ϴ�. StepTimer�� ���� ���� ������ �����ϰ�(Update) ȭ���� �׸��� ����(Render)�� �����մϴ�.
/// Render()�� �� ���۸� �������� �� Device Resources���� ���� ����ȭ ���θ� Ȯ���� �� �����ð��� �����Ͽ� Present()�� ȣ���ϹǷ� �� ƽ���� ����˴ϴ�.
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

// StepTimer�� ���� �� �����Ӹ��� ȣ��˴ϴ�. Ű������ ���¸� �����ϰ� Ư�� Ű �Է¿� ���� ������ ȣ���մϴ�.
void Game::Update(DX::StepTimer& timer)
{
	auto kb = m_keyboard->GetState();

	// kb.KeyCode : KeyCode�� �ش��ϴ� Ű�� ���ȴٸ� true�� ��ȯ�Ѵ�.
	if (kb.Escape)
	{
		ExitGame();
	}
}

#pragma endregion

#pragma region Frame Render

/// <summary>
/// ȭ���� �׸��� ������ �����ϸ� �� ƽ���� ȣ��˴ϴ�. ȭ���� Ŭ�����ϰ� �� ���ۿ� �׸��� �׸� ��, ���� ü���� Present()�� ȣ���մϴ�.
/// ���� ����ȭ ���θ� Ȯ���ϴ� �۾��� DeviceResources::Present()���� �����մϴ�.
/// </summary>
void Game::Render()
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

	m_spriteBatch->Draw(m_texCat.Get(), XMFLOAT2(50.f, 50.f));
	m_spriteBatch->Draw(
		m_texBug.Get(),
		XMFLOAT2(0.f, 0.f),
		nullptr,
		Colors::White,
		0.f,				// ȸ��
		XMFLOAT2(0.f, 0.f), // ����
		3.0f				// ũ��
	);

	m_spriteBatch->End();
	//-------------------------------------------------------------

	m_deviceResources->PIXEndEvent();

	m_deviceResources->Present();
}

// ȭ���� �׸��� ���� �� ���ۿ� ����� View�� �ʱ�ȭ�ϰ�, ���� Ÿ�ٰ� ����Ʈ�� �����մϴ�.
void Game::Clear()
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
void Game::OnActivated()
{
}

// WM_ACTIVATEAPP �޽����� ���� ���� ��Ȱ��ȭ �� �۵��մϴ�.
void Game::OnDeactivated()
{
}

// �Ͻ����� �� ȣ��˴ϴ�. â �ּ�ȭ �� ���� ��� ���� �� �ڵ����� ȣ��˴ϴ�.
void Game::OnSuspending()
{
}

// �Ͻ����� ���� �� ȣ��˴ϴ�. â �ּ�ȭ ���� �� ���� ��� ���� �� �ڵ����� ȣ��˴ϴ�.
void Game::OnResuming()
{
	m_timer.ResetElapsedTime();
}

// ���α׷��� �ٸ� ���÷��̷� �̵����� �� �۵��մϴ�. DeviceResources�� ���� â ũ�⸦ �Ѱܼ� ���� ������ ������Ʈ�մϴ�.
void Game::OnWindowMoved()
{
	auto rct = m_deviceResources->GetOutputSize();
	m_deviceResources->OnWindowSizeChanged(rct.right, rct.bottom);
}

/// <summary>
/// ���α׷��� â ũ�Ⱑ ����Ǿ��� �� �ܺο��� ȣ��˴ϴ�. DeviceResources�� ����� â ũ�⸦ �Ѱܼ� ���� ������ ������Ʈ�մϴ�.
/// ���� â ũ�Ⱑ ����Ǿ��ٸ� DeviceResources���� �ػ� ���� ���ҽ��� ������Ǹ�, ���� �� ���ӿ��� ���Ǵ� ���ҽ��鵵 ������մϴ�.
/// </summary>
void Game::OnWindowSizeChanged(int width, int height)
{
	if (m_deviceResources->OnWindowSizeChanged(width, height))
	{
		CreateWindowSizeDependentResources();
	}
}

// ��� ��� ����ϴ� �Լ�. ������ ũ���� ����Ʈ���� ��ȯ�մϴ�.
void Game::GetDefaultSIze(int& width, int& height) const noexcept
{
	width = 800;
	height = 600;
}

#pragma endregion

#pragma region Direct3D Resources

// �� ���ӿ��� ���Ǵ� ��ġ ���� ���ҽ��� �����մϴ�.
void Game::CreateDeviceDependentResources()
{
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_commonStates = std::make_unique<CommonStates>(device);
	m_spriteBatch = std::make_unique<SpriteBatch>(context);

	// CreateWICTextureFromFile: ���ϸ��� �Է��ϸ� WIC�� ���� ��ȯ�� ��, ���ҽ� �� ���ҽ� �信 ��ȯ�մϴ�.
	//	�� ���ҽ��� ���� ���ҽ��� ����(ex: float, float, float), ���ҽ� ��� Ư�� ����(ȭ�� ǥ��)�� ���� ��ȯ�� ����(ex: r,g,b �Ǵ� x,y,z)
	//	�� ���⼭�� �ؽ��� ��ü�� ������ ���� ������ ���ҽ� �丸 �޴´�.
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Assets/bug1_1.png", nullptr, m_texBug.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Assets/cat.png", nullptr, m_texCat.ReleaseAndGetAddressOf()));

	// ����: �� ���� �ҷ��� �� �ִ� �̹��� ũ�⿡�� ������ �ִ�. 
	//		(DirectX 9������ 2048x2048, 11������ 16384x16384. ��, PC�� DirectX ���� ���θ� ���� ������, �޴����� ������ ���� �ִ�ġ�� ������ �־ 4096 x 4096 �� �ּһ�� �������� �ڸ���.)
}

// �� ���ӿ��� ���Ǵ� �ػ� ���� ���ҽ��� �����մϴ�.
void Game::CreateWindowSizeDependentResources()
{

}

// ��ġ �ս� �� �� ���ӿ��� ó���� �����Դϴ�. 
void Game::OnDeviceLost()
{
	m_texCat.Reset();
	m_texBug.Reset();

	m_spriteBatch.reset();
	m_commonStates.reset();
}

// ��ġ ���� �� �� ���ӿ��� ó���� �����Դϴ�. ���ҽ��� �ٽ� �����մϴ�.
void Game::OnDeviceRestored()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

#pragma endregion
