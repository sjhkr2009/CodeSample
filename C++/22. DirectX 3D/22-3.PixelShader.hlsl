
// �Ʒ��� ���� ����ü�� �Է¹޾Ƶ� �ǰ�
struct Input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// �׳� 2���� �Ű������� �޾Ƶ� �ȴ�. ������ �� DirectX�� �˾Ƽ� ����ü�� �� �Ű� ������ �����Ǵ� ��� ������ Ǯ� �ؼ��Ѵ�.
float4 main(float4 pos : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}