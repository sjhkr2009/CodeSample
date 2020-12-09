// Vertex Shader: Vertex의 색상과 위치값 조정 (Transform, Lighting)

cbuffer MatrixBuffer
{
    matrix worldMatrix;
};

struct VS_Output
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_Output main(float4 pos : POSITION, float2 tex : TEXCOORD0)
{
    VS_Output o;

    // 왼쪽은 행 벡터, 오른쪽은 열 벡터로 취급한다. 
    // 매개변수의 순서를 바꾸면(worldMatrix, pos) 전치(Transpose)를 하지 않고 넘겨줘도 되지만, 여러 개의 행렬 연산에서는 헷갈릴 수 있으니 전치를 해서 넘기는 게 좋다.
    o.position = mul(pos, worldMatrix);
    o.tex = (tex * 1.1f) - 0.05f;

	return o;
}
