struct Output
{
	float4 position : SV_POSITION;
	// �ؽ��ĸ� ���� ���̹Ƿ� ���� ��� �ؽ��� ����(u,v)�� ���� ������ float2�� �����Ѵ�. Semantic ��ȣ�� �ؽ��� �ڵ� 0��(TEXCOORD0)
	float2 tex : TEXCOORD0;
};

Output main( float4 pos : POSITION, float2 tex : TEXCOORD0)
{
	Output o;

	o.position = pos;
	o.tex = tex;

	return o;
}