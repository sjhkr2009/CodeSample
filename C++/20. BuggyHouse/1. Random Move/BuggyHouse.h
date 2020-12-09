#pragma once

#include <list>
#include <memory>
#include "D2DFramework.h"
#include "Bug.h"

class BuggyHouse : public D2DFramework
{
public:
	std::unique_ptr<Actor> mspBackground;
	std::list<std::unique_ptr<Bug>> mBugList{};

public:
	HRESULT Initialize(
		HINSTANCE hInstance,
		LPCWSTR title = L"Buggy House",
		UINT width = 1024,
		UINT height = 768
	) override;
	void Release() override;
	void Render() override;
};

