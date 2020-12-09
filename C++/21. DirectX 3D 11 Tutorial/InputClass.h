#pragma once

class InputClass
{
private:
	bool mKeys[256];

public:
	InputClass() {}
	InputClass(const InputClass&) {}
	~InputClass() {}

	void Init();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
};