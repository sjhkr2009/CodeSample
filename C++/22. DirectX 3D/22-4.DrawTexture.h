#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>

#pragma comment (lib, "d3dcompiler.lib")

// BMP 파일 텍스쳐로 읽기

class DrawTexture :
    public D3DFramework
{
    struct Vertex
    {
        // 텍스쳐 적용을 위해 색상 대신 u, v 값이 필요하다.
        FLOAT x, y, z;
        FLOAT u, v;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer>        mspVertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>   mspInputLayout;
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader>  mspVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   mspPixelShader;

    // 텍스쳐 저장 공간과 뷰를 선언한다.
    Microsoft::WRL::ComPtr<ID3D11Texture2D>             mspTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>    mspTextureView;

    // 픽셀 셰이더 샘플러 제어를 위한 인터페이스
    Microsoft::WRL::ComPtr<ID3D11SamplerState>          mspSamplerState{};

    // 알파 블렌딩 설정을 위한 인터페이스
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

