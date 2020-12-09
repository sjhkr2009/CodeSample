#include "SystemClass.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR pScmdline,
	int iCmdshow
)
{
	SystemClass* System;
	bool result;

	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	//TODO: Init() and Run()
}