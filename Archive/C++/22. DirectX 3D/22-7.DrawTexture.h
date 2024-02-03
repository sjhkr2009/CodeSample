#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3dcompiler.lib")

// 이미지 조작하기

class DrawTexture :
    public D3DFramework
{
    struct Vertex
    {
        FLOAT x, y, z;
        FLOAT u, v;
    };

    struct MatrixBuffer
    {
        DirectX::XMMATRIX world;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>        mspVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   mspInputLayout;
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader>  mspVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   mspPixelShader;

    Microsoft::WRL::ComPtr<ID3D11Texture2D>             mspTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    mspTextureView;

    Microsoft::WRL::ComPtr<ID3D11SamplerState>          mspSamplerState{};
    Microsoft::WRL::ComPtr<ID3D11BlendState>            mspBlendState{};

    // 행렬이 들어갈 상수 버퍼. 상수 버퍼도 그냥 버퍼 타입으로 만든다.
    Microsoft::WRL::ComPtr<ID3D11Buffer>    mspConstBuffer;

    // 조작할 그림의 좌표와 회전값. x,y축에 있는 2D그림이므로 회전은 z축으로 조작한다.
    float mX, mY;
    float mRotationZ;

    // 월드 좌표계 값을 기록할 행렬
    DirectX::XMMATRIX mWorld;

public:
    void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
    void Destroy() override;

protected:
    void Render() override;
    void Update(float delta) override;

private:
    void InitTriangle();
    void InitPipeline();

    HRESULT CreateTextureFromBMP();
};
