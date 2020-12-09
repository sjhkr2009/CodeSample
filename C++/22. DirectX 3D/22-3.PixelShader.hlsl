
// 아래와 같은 구조체를 입력받아도 되고
struct Input
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

// 그냥 2개의 매개변수를 받아도 된다. 컴파일 시 DirectX가 알아서 구조체를 각 매개 변수에 대응되는 멤버 변수로 풀어서 해석한다.
float4 main(float4 pos : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}