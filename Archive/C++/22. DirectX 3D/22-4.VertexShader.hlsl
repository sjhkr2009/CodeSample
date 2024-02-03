struct Output
{
	float4 position : SV_POSITION;
	// 텍스쳐를 넣을 것이므로 색상 대신 텍스쳐 정보(u,v)를 넣을 공간을 float2로 선언한다. Semantic 기호는 텍스쳐 코드 0번(TEXCOORD0)
	float2 tex : TEXCOORD0;
};

Output main( float4 pos : POSITION, float2 tex : TEXCOORD0)
{
	Output o;

	o.position = pos;
	o.tex = tex;

	return o;
}