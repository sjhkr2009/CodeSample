#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"
#include <array>

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

	// 추가된 변수 및 함수
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texCat;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texBug;
	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>				m_commonStates;

	std::array<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>, 10> m_textures;
	double m_timeToNextFrame{ 0.1 };
	int m_currentFrame{};
};

