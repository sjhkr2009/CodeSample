#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>    // 셰이더 컴파일을 위한 라이브러리

#pragma comment (lib, "d3dcompiler.lib")

class DrawTriangle :
    public D3DFramework
{
    struct Vertex
    {
        // 꼭지점은 위치(x,y,z)와 색상(r,g,b,a)값이 필요하다.
        FLOAT x, y, z;
        FLOAT color[4];
    };

    // 파이프라인에 태워 보낼 데이터는 마이크로소프트에서 지정한 버퍼를 사용해야 한다. (배열은 받지 않음)
    Microsoft::WRL::ComPtr<ID3D11Buffer> mspVertexBuffer;

    // Vertex Buffer 구조가 어떻게 구성되어 있는지 파이프라인에 넘겨준다. 이 정보를 참고하여 버퍼를 해석하게 된다. (Input Assemble 단계)
    Microsoft::WRL::ComPtr<ID3D11InputLayout> mspInputLayout;
    
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mspVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mspPixelShader;

public:
    void Initialize(HINSTANCE hInstance, int width = 800, int height = 600) override;
    void Destroy() override;

protected:
    void Render() override;

private:
    void InitTriangle();
    void InitPipeline();
};

