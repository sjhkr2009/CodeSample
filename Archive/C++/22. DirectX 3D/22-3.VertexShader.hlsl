// SV (System Value) : ������ ������������ ���� ���� �ܰ�� �������� ������ SV�� �ٿ� �����Ѵ�. (�ٿ��� �Ѿ�°� �ƴ�)
// float4�� ��ġ(position), ����(color), ���(target) �� �پ��� �������� ǥ���� ���� �� �־, �ڿ� : POSITION �� ���� �뵵�� �ٿ��ش�.
//	�� �̷��� ���� Ÿ�������� �ٸ��� ���Ǵ� ���� 'Semantic ����'�̶�� �Ѵ�.

// ���⼭�� ��ġ�� ���� ������ �ޱ�� �Ѵ�.

// ��ȯ������ 2��(��ġ,����)�� ��ȯ�ؾ� �ϹǷ� ����ü�� �����.
struct Output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// ������ ó�� ���� ���� ���� �״�� �Ѱ��ֱ�� �Ѵ�.
Output main( float4 pos : POSITION, float4 color : COLOR )	// ����ü ������� �ǹ� ��ȣ( : SOMETHING)�� ������ ������ ����� �ʿ����
{
	Output o;

	o.position = pos;
	o.color = color;

	return o;
}