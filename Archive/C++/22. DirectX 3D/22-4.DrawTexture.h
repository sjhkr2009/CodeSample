#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")

// BMP ���� �ؽ��ķ� �б�

class DrawTexture :
    public D3DFramework
{
    struct Vertex
    {
        // �ؽ��� ������ ���� ���� ��� u, v ���� �ʿ��ϴ�.
        FLOAT x, y, z;
        FLOAT u, v;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>        mspVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   mspInputLayout;
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader>  mspVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   mspPixelShader;

    // �ؽ��� ���� ������ �並 �����Ѵ�.
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             mspTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    mspTextureView;

    // �ȼ� ���̴� ���÷� ��� ���� �������̽�
    Microsoft::WRL::ComPtr<ID3D11SamplerState>          mspSamplerState{};

    // ���� ���� ������ ���� �������̽�
    Microsoft::WRL::ComPtr<ID3D11BlendState>            mspBlendState{};

public:
    void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
    void Destroy() override;

protected:
    void Render() override;

private:
    void InitTriangle();
    void InitPipeline();

    HRESULT CreateTextureFromBMP();
};

