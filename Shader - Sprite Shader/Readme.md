# Custom Sprite Shader

2D 환경에서 Sprite에 간단한 효과를 적용하는  커스텀 셰이더와 그에 대한 에디터를 포함하고 있습니다. 다음과 같은 기능을 지원합니다.

- **두들 애니메이션(Doodle Animation)**: 텍스처에 동적인 움직임을 추가하여 두들 같은 애니메이션 효과를 줍니다.
- **HSV 색상 조정(HSV Edit)**: 텍스쳐의 Hue(색조), Saturation(채도), Brightness(명도)를 조절할 수 있습니다.
- **그라디언트 오버레이(Gradient Overlay)**: 텍스처에 4색, 세로, 가로, 원형 그라디언트 효과를 추가할 수 있습니다.
- **그림자 효과(Shadow)**: 텍스처에 그림자를 추가합니다. 그림자의 방향과 색상, 가장자리 보간 효과를 조절할 수 있습니다.
- **빛 효과(Shine Effect)**: 텍스처에 일정 시간마다 움직이는 광택 효과를 추가합니다. 빛의 속도, 간격, 방향, 너비 등을 조절할 수 있습니다.

## 사용 방법

### 셰이더 적용하기

1. Unity 에디터에서 셰이더를 사용할 `Material`을 생성합니다.
2. 생성된 `Material`의 Shader 드롭다운 메뉴에서 `Custom/CustomShader`를 선택합니다. 셰이더 이름은 원하는대로 변경해도 됩니다.