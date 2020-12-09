#pragma once

#include <queue>

// 상속 가능하게 변경(final)
// private->protected, 소멸자 및 멤버 함수 virtual 형태로 변경
class Game : public DX::IDeviceNotify
{
public:
	Game() noexcept(false);
	virtual ~Game();
	
	Game(Game&&) = default;
	Game& operator =(Game&&) = default;

	Game(Game const&) = delete;
	Game& operator = (Game const&) = delete;

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

	std::unique_ptr<DX::DeviceResources> m_deviceResources;
	DX::StepTimer m_timer;

	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	std::unique_ptr<DirectX::Mouse> m_mouse;

	std::unique_ptr<DirectX::SpriteBatch>				m_spriteBatch;
	std::unique_ptr<DirectX::CommonStates>				m_commonStates;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_texture;

	double				m_timeToNextFrame{ 0.1 };
	//int					m_currentFrame{ 0 };
	//std::vector<RECT>	m_rects;
	
	struct Frame
	{
		const wchar_t* spriteName;
		double duration;
	};
	std::queue<Frame> m_animQueue{};
	RECT m_rect{};
	bool canAddAnim{ true };
	double addAnimCooldown{};

	rapidjson::GenericDocument<rapidjson::UTF16<>> animationDoc;
	rapidjson::GenericDocument<rapidjson::UTF16<>> spriteSheetDoc;

	//void LoadSpriteSheetsFromJson();
	void SetAnimation(const wchar_t* name);
	RECT GetRectByFilename(const wchar_t* spriteName);
	void GetFrameInfo(const wchar_t* animName, int index, double& duration, RECT& rect);
	rapidjson::GenericDocument<rapidjson::UTF16<>> LoadJson(const wchar_t* filename);
};
