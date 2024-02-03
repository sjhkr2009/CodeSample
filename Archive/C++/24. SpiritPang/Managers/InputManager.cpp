#include "PCH.h"

using namespace DX;
using namespace DirectX;

void InputManager::Initialize(HWND hwnd)
{
	m_keyboard = std::make_unique<Keyboard>();

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetWindow(hwnd);
}

void InputManager::Reset()
{
	m_keyState.Reset();
	m_mouseState.Reset();
}

void InputManager::Update()
{
	const auto& keyInfo = m_keyboard->GetState();
	const auto& mouseInfo = m_mouse->GetState();

	m_keyState.Update(keyInfo);
	m_mouseState.Update(mouseInfo);

	mouseX = mouseInfo.x;
	mouseY = mouseInfo.y;
}
