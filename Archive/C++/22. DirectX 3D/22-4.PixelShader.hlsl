Texture2D shaderTexture;
SamplerState Sampler;	// C++ 코드에서 샘플러 제어하기

struct PsInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PsInput input) : SV_TARGET
{
    float4 color = shaderTexture.Sample(Sampler, input.tex);
	
    // color[0] = 0;	// float4의 0,1,2,3번에 픽셀 포맷에 맞는 r,g,b,a  값이 0~1로 들어온다.
	// 예를 들어, 0번 인덱스 값을 0으로 만들면 blue값이 0이 된다.
	
	// Grayscale Formula : 회색조로 만드는 공식을 적용해본다. (각 색상값을 0.3R + 0.59G + 0.11B 값으로 수정한다)
    float gray = (color[0] * 0.3f) + (color[1] * 0.59f) + (color[2] * 0.11f);
	
	// 흑백으로 만드려면 회색조로 된 값에서 0.5 이상이면 검은색, 미만이면 흰색으로 처리한다.
    float black = (gray >= 0.5f) ? 1.f : 0.f;
	
	// 테스트
    //color[0] = color[1] = color[2] = black;
    //color[0] = color[1] = color[2] = gray;
	
    return color;
}