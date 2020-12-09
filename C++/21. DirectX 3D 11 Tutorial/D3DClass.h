#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
private:
	bool	mVsyncEnabled;
	int		mVideoCardMemory;
	char	mVideoCardDescription[128];

	IDXGISwapChain*				mSwapChain;
	ID3D11Device*				mDevice;
	ID3D11DeviceContext*		mDeviceContext;
	ID3D11RenderTargetView*		mRenderTargetView;
	ID3D11Texture2D*			mDepthStencilbuffer;
	ID3D11DepthStencilState*	mDepthStencilState;
	ID3D11DepthStencilView*		mDepthStencilView;
	ID3D11RasterizerState*		mRasterState;

	XMMATRIX mProjectionMatrix;
	XMMATRIX mWorldMatrix;
	XMMATRIX mOrthoMatrix;

public:
	D3DClass();
	D3DClass(const D3DClass&) {};
	~D3DClass() {};

	bool Init(int, int, bool, HWND, bool, float, float);
	void Shutdown();

	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	
	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);
};

