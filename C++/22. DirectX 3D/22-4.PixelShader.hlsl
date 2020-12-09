Texture2D shaderTexture;
SamplerState Sampler;	// C++ �ڵ忡�� ���÷� �����ϱ�

struct PsInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PsInput input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(Sampler, input.tex);
	
    // color[0] = 0;	// float4�� 0,1,2,3���� �ȼ� ���˿� �´� r,g,b,a  ���� 0~1�� ���´�.
	// ���� ���, 0�� �ε��� ���� 0���� ����� blue���� 0�� �ȴ�.
	
	// Grayscale Formula : ȸ������ ����� ������ �����غ���. (�� ������ 0.3R + 0.59G + 0.11B ������ �����Ѵ�)
    float gray = (color[0] * 0.3f) + (color[1] * 0.59f) + (color[2] * 0.11f);
	
	// ������� ������� ȸ������ �� ������ 0.5 �̻��̸� ������, �̸��̸� ������� ó���Ѵ�.
    float black = (gray >= 0.5f) ? 1.f : 0.f;
	
	// �׽�Ʈ
    //color[0] = color[1] = color[2] = black;
    //color[0] = color[1] = color[2] = gray;
	
    return color;
}