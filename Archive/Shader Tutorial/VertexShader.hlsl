cbuffer cBuffer
{
    float time;
}

struct Vertex
{
	float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct Interpolants
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//셰이더의 두 가지 분류: 버텍스 셰이더, 프래그먼트 셰이더
//vertex shader : 3차원 공간의 각 버텍스마다 호출된다

//셰이더는 모두 각 버텍스, 각 픽셀에 독립적으로 호출된다. 한 버텍스/픽셀에 적용된 내용이 다른 버텍스/픽셀에 함께 적용되지 않는다.

Interpolants main( Vertex In )
{
    // 버텍스의 각 좌표값이 In으로 넘어온다. 원하는 대로 수정해서 Out 좌표를 내보낸다.

    Interpolants Out;

    Out.position = In.position;

    // 1. 시간에 따라 -0.1 ~ 0.1 좌표를 왕복하게 하기
    // 스칼라 값을 연산할 때는 예상치 못한 값이 변경되지 않게 유의할 것. x,y 값만 바뀌도록 float4를 곱해준다.
    //Out.position += sin(time) * float4(0.1, 0.1, 0.0, 0.0);

    // 2. 특정 축만 대입도 가능. x를 사인, y를 코사인값에 따라 변경시키면 원을 그리게 된다.
    //Out.position.x += sin(time); 
    //Out.position.y += cos(time);

    // 3. 여기서 내 현재 위치를 더하면 물결이 치는 효과를 줄 수 있다. 
    //Out.position.x += sin(time*3 + In.position.x) * 0.1; 
    //Out.position.y += cos(time*3) * 0.1;

    // 4. x,y 값을 모두 더해주면 모든 방향으로 울렁이는 효과
    // 좀더 빠르게 보기 위해 사인/코사인함수를 3배 빠른 주기로 요동치게 해본다.
    //Out.position.x += sin(time*3 + In.position.x + In.position.y) * 0.1; 
    //Out.position.y += cos(time*3 + In.position.x + In.position.y) * 0.1;

    // 사인/코사인에 현재 세타값을 대입해서 어떤 결과가 나올지는 예측하기 어렵다.
    // 다만 눈으로 결과를 볼 수 있으니 이것저것 바꿔보면 예상치 않게 좋은 결과가 나오기도 하니, 다양하게 시도하고 좋은 건 기록해둘 것. (탄젠트 계열은 잘 안쓴다)

    // 5. if문을 적용해서 0보다 높은 y값에 대해서만 사인값 바리에이션 주기
    if(Out.position.y > 0.0)
    {
        //Out.position.x += sin(time*3) * 0.1;
    }
    
    
    Out.tex = In.tex;
    
	return Out;
}