#pragma once
#include "D3DFramework.h"
#include <d3dcompiler.h>    // ���̴� �������� ���� ���̺귯��

#pragma comment (lib, "d3dcompiler.lib")

class DrawTriangle :
    public D3DFramework
{
    struct Vertex
    {
        // �������� ��ġ(x,y,z)�� ����(r,g,b,a)���� �ʿ��ϴ�.
        FLOAT x, y, z;
        FLOAT color[4];
    };

    // ���������ο� �¿� ���� �����ʹ� ����ũ�μ���Ʈ���� ������ ���۸� ����ؾ� �Ѵ�. (�迭�� ���� ����)
    Microsoft::WRL::ComPtr<ID3D11Buffer> mspVertexBuffer;

    // Vertex Buffer ������ ��� �����Ǿ� �ִ��� ���������ο� �Ѱ��ش�. �� ������ �����Ͽ� ���۸� �ؼ��ϰ� �ȴ�. (Input Assemble �ܰ�)
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

