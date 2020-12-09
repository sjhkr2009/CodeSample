#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
private:
	LPCWSTR mApplicationName{};
	HINSTANCE mHinstance{};
	HWND mHwnd{};

	InputClass* mInput;
	GraphicsClass* mGraphics;

public:
	SystemClass();
	SystemClass(const SystemClass&) {};
	~SystemClass() {};

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
};

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle{ nullptr };