// SV (System Value) : 렌더링 파이프라인을 따라 다음 단계로 보내지는 값들은 SV를 붙여 구분한다. (붙여야 넘어가는건 아님)
// float4는 위치(position), 색상(color), 대상(target) 등 다양한 데이터의 표현에 사용될 수 있어서, 뒤에 : POSITION 과 같이 용도를 붙여준다.
//	ㄴ 이렇게 같은 타입이지만 다르게 사용되는 것을 'Semantic 문법'이라고 한다.

// 여기서는 위치와 색상 정보를 받기로 한다.

// 반환값으로 2개(위치,색상)를 반환해야 하므로 구조체로 만든다.
struct Output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 별도의 처리 없이 받은 값을 그대로 넘겨주기로 한다.
Output main( float4 pos : POSITION, float4 color : COLOR )	// 구조체 멤버들이 의미 기호( : SOMETHING)를 가지고 있으니 여기는 필요없음
{
	Output o;

	o.position = pos;
	o.color = color;

	return o;
}