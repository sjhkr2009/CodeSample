#pragma once

// GameBase: ������ �⺻���� ������ �����մϴ�. ���� GameManager�� �ش��ϴ� Ŭ������ �ִٸ� ��ӹ޾� ����ϼ���.

class GameBase : public DX::IDeviceNotify
{
public:
	static const int screenHeight{ 800 };
	static const int screenWidth{ 1200 };

	enum class Layer : int
	{
		Background = 0,
		BackgroundUI = 1,
		Character = 2,
		Frame = 3,
		Particle = 4,
		UI = 5,
		UIText = 6
	};

public:
	GameBase() noexcept(false);
	virtual ~GameBase();

	GameBase(GameBase&&) = default;
	GameBase& operator =(GameBase&&) = default;

	GameBase(GameBase const&) = delete;
	GameBase& operator = (GameBase const&) = delete;

	virtual void Initialize(HWND hwnd, int width, int height);

	virtual void Tick();

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void OnActivated();
	virtual void OnDeactivated();
	virtual void OnSuspending();
	virtual void OnResuming();

	virtual void OnWindowMoved();
	virtual void OnWindowSizeChanged(int width, int height);

	virtual void GetDefaultSIze(int& width, int& height) const noexcept;

protected:
	virtual void Update(DX::StepTimer const& timer);
	virtual void Render();

	virtual void Clear();

	virtual void CreateDeviceDependentResources();
	virtual void CreateWindowSizeDependentResources();

protected:
	std::unique_ptr<DX::DeviceResources>	m_deviceResources;
	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::Keyboard>		m_keyboard;
	std::unique_ptr<DirectX::Mouse>			m_mouse;

	DirectX::Keyboard::KeyboardStateTracker m_keyState;
	DirectX::Mouse::ButtonStateTracker		m_mouseState;

	std::unique_ptr<DirectX::SpriteBatch>	m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>	m_commonStates;
};
