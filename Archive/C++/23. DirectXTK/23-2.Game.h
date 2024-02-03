#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"

class Game final : public DX::IDeviceNotify
{
public:
	Game() noexcept(false);
	~Game();
	
	Game(Game&&) = default;
	Game& operator =(Game&&) = default;

	Game(Game const&) = delete;
	Game& operator = (Game const&) = delete;

	void Initialize(HWND hwnd, int width, int height);

	void Tick();

	void OnDeviceLost() override;
	void OnDeviceRestored() override;

	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();

	void OnWindowMoved();
	void OnWindowSizeChanged(int width, int height);

	void GetDefaultSIze(int& width, int& height) const noexcept;

private:
	void Update(DX::StepTimer& timer);
	void Render();

	void Clear();

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	std::unique_ptr<DX::DeviceResources> m_deviceResources;
	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;
};

