// Vertex Shader: Vertex�� ����� ��ġ�� ���� (Transform, Lighting)

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

    // ������ �� ����, �������� �� ���ͷ� ����Ѵ�. 
    // �Ű������� ������ �ٲٸ�(worldMatrix, pos) ��ġ(Transpose)�� ���� �ʰ� �Ѱ��൵ ������, ���� ���� ��� ���꿡���� �򰥸� �� ������ ��ġ�� �ؼ� �ѱ�� �� ����.
    o.position = mul(pos, worldMatrix);
    o.tex = (tex * 1.1f) - 0.05f;

	return o;
}
