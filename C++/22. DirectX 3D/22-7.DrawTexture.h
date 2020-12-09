#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3dcompiler.lib")

// �̹��� �����ϱ�

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

    // ����� �� ��� ����. ��� ���۵� �׳� ���� Ÿ������ �����.
    Microsoft::WRL::ComPtr<ID3D11Buffer>    mspConstBuffer;

    // ������ �׸��� ��ǥ�� ȸ����. x,y�࿡ �ִ� 2D�׸��̹Ƿ� ȸ���� z������ �����Ѵ�.
    float mX, mY;
    float mRotationZ;

    // ���� ��ǥ�� ���� ����� ���
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
