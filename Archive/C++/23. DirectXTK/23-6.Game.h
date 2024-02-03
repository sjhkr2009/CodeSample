#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"
#include <vector>

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
	std::vector<ID3D11ShaderResourceView*>				m_bugDieAnim;
	int													m_bugDieAnimFrameIndex{};
	int													m_bugDieAnimStartTime{};
	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>				m_commonStates;

	inline void DrawAnimation(std::vector<ID3D11ShaderResourceView*> anim, float intervalTime, float x, float y, int& frameIndex, int& startTime, bool isLoop = true, int startFrame = 0);
};

