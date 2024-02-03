#pragma once

namespace DX
{
	class InputManager
	{
	public:
		static InputManager& Instance()
		{
			static InputManager instance;
			return instance;
		}

		InputManager() {}
		~InputManager() {}

		InputManager(InputManager const&) = delete;
		InputManager& operator =(InputManager const&) = delete;

		InputManager(InputManager&&) = delete;
		InputManager& operator =(InputManager&&) = delete;

	public:
		void Initialize(HWND hwnd);
		void Reset();
		void Update();

		bool IsPointerDown() { return m_mouseState.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		bool IsPointerUp() { return m_mouseState.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED; }
		DirectX::XMFLOAT2 XM_CALLCONV GetMousePos() { return DirectX::XMFLOAT2{ static_cast<float>(mouseX), static_cast<float>(mouseY) }; }

		bool IsKeyDown(DirectX::Keyboard::Keys key) { return m_keyState.IsKeyPressed(key); }

	private:
		std::unique_ptr<DirectX::Keyboard>		m_keyboard;
		std::unique_ptr<DirectX::Mouse>			m_mouse;

		DirectX::Keyboard::KeyboardStateTracker m_keyState;
		DirectX::Mouse::ButtonStateTracker		m_mouseState;

		int mouseX{};
		int mouseY{};
	};
}

