cbuffer cBuffer
{
    float time;
}
Texture2D shaderTexture;
SamplerState Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct Interpolants
{
	float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

struct Pixel
{
	float4 color : SV_TARGET; // 색상은 float을 쓴다. 즉 색상은 0~255가 아니라 0~1 사이로 정규화된 값을 쓴다.
};

//셰이더의 두 가지 분류: 버텍스 셰이더, 프래그먼트 셰이더
//fragment shader (= pixel shader) : 모니터의 각 픽셀마다 호출된다

//셰이더는 모두 각 버텍스, 각 픽셀에 독립적으로 호출된다. 한 버텍스/픽셀에 적용된 내용이 다른 버텍스/픽셀에 함께 적용되지 않는다.

Pixel main(Interpolants In)
{
    // 렌더링 파이프라인 상에서 래스터라이즈 된 텍스쳐의 각 점(pixel)이 넘어온다.
    // u,v 좌표계에서 동작하며 In으로 들어온 점의 색상에 원하는 변화를 가해 Out으로 내보낸다.

    Pixel Out;
    Out.color = shaderTexture.Sample(Sampler, In.tex);

    // 1. 특정 색상 강조
    // Out.color.r = 1;

    // 2. 그레이스케일
    // 대부분 r,g,b 값을 평균내서 만든다. 알파값은 안 바뀌게 rgb 값에 대입할 것. 
    //Out.color.rgb = (Out.color.r + Out.color.g + Out.color.b) / 3;

    // 3. 시간에 따른 색상 변화
    // 사인/코사인 사용 시 색상값이 0~1 사이임에 유의. 사인/코사인은 -1~1 사이를 움직이므로 1을 더하고 2로 나누면 0~1 범위가 된다.
    //Out.color.r += (sin(time) + 1) / 2;

    // 4. u,v 좌표계는 왼쪽 위를 원점으로 (0,0) ~ (1,1) 사이의 값을 갖는 2차 벡터이다. In.tex에는 현재 In에 들어온 점의 u,v 좌표가 들어온다.
    // 좌표에 따라 색상값을 바꿔주기만 해도 그라데이션을 구현 가능. 
    //Out.color = float4(In.tex, 0, 1);

    // 5. 원점에서의 거리에 따른 그라데이션도 가능. length 으로 원점에서의 거리를 얻을 수 있다.
    //float2 p = In.tex;
    //Out.color = length(p);
    //Out.color.a = 1;

    // 6. 원형 그라데이션은 중앙(0.5, 0.5)을 기준으로 하면 된다.
    //float2 p = In.tex;
    //Out.color = length(p - float2(0.5, 0.5));
    //Out.color.a = 1;
    
    // 7. 원본 색상에 색깔을 더하면 밝아진다. (Additive 연산)
    //float2 p = In.tex;
    //Out.color.rgb += length(p - float2(0.5, 0.5));
    //Out.color.a = 1;

    // 8. 색상을 곱하면 어두워진다. (Multiple 연산)
    // 0~1 사이의 값은 곱할수록 작아지기 때문. 다만 0이 섞여있으면 항상 0이 됨에 주의. 
    //float2 p = In.tex;
    //Out.color.rgb *= length(p - float2(0.5, 0.5));
    //Out.color.a = 1;

    // 9. 그라데이션을 반전시키려면 거리 계산을 반대로 하면 된다. uv 값은 0~1 사이 값이므로 1에서 값을 빼면 반전됨.
    //float2 p = In.tex;
    //Out.color.rgb *= 1 - length(p - float2(0.5, 0.5));
    //Out.color.a = 1;

    // 10. 시간에 따른 변화도 가능
    //float2 p = In.tex;
    //Out.color = 1 - length(p - float2(0.5, 0.5)) * (sin(time) * 2);
    //Out.color.a = 1;

    // 11. 원형 그라데이션을 회전시키기. 거리를 구할 지점 p의 좌표를 조작하면 가능하다.
    //float2 p = In.tex;
    //p.x += sin(time * 2);
    //p.y += cos(time * 2);
    //Out.color += 1 - length(p - float2(0.5, 0.5));
    //Out.color.a = 1;

    // 12. 복잡한 수학적 계산이 필요한 기하학적 그림은 외부 사이트에서 공식을 참고해도 된다. shader pattern 같은걸로 검색하면 많이 나옴.
    // 예를 들면 이런거 >> https://plus.maths.org/content/counting-rose-petals
    // 예제 - rose petal (꽃잎 모양 그리기)
    In.tex = In.tex * 2 - 1; // -1 ~ 1 사이의 값으로 만든다.

    float dist = length(In.tex); // uv 길이
    float size = 1; // 꽃 크기
    float petals = 6; // 꽃잎 개수
    float thick = 0.3; // 꽃잎 길이
    float inset = 0.8; // 꽃잎이 튀어나온 정도(0이면 원, 1이면 중심부가 없는 꽃잎)

    // atan2(y,x)로 거리를 통해 부채꼴의 각도를 구한다
    if(dist <= thick * (size + inset * sin(petals * atan2(In.tex.y, In.tex.x))))
    {
        Out.color = float4(1,1,1,1);
    }

    // 셰이더를 통해 고해상도의 텍스쳐나 많은 애니메이션 없이도 기하학적인 모형을 쉽게 그릴 수 있다.
    // 예를 들어 게임에서 스킬 범위를 표현하는 등
    
    return Out;

}