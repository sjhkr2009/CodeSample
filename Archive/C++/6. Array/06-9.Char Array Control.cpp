#include <iostream>

int main()
{
	// 1. 문자열 비교하기
	
	char stringA[6]{ "Hello" };
	char stringB[6]{ "HEllo" };

	// 기존 방식 (for문 이용)
	bool isEqual = true;
	for (int i = 0; i < 6; i++)
	{
		if (stringA[i] != stringB[i])
		{
			isEqual = false;
			break;
		}
	}

	// string compare 함수 이용 - 두 문자열을 입력받고, 양수, 0, 음수 중 하나를 출력한다.
	strcmp(stringA, stringB);
	// < 0 (음수) : stringA 가 더 작다 (아스키코드 상으로)
	// > 0 (양수) : stringB 가 더 작다 
	// = 0 : 두 문자열이 같다

	if (!strcmp(stringA, stringB)) // strcmp(stringA, stringB) == 0 과 같은 의미. 0이 false를 의미하기 때문.
	{
		// 두 문자열이 같을 때 실행
	}
	


	//--------------------------------------------------------------------------------------------------------

	// 2. 문자열 길이 구하기

	char stringC[]{ "Hello" };

	// 기존 방식 (for문 이용)
	int index{ 0 };
	while (stringC[index] != '\0')
	{
		index++;
	}

	// string length 함수 이용 - 문자열을 입력받아 개수를 반환한다.
	strlen(stringC);
	

	//--------------------------------------------------------------------------------------------------------


	// 3. 문자열 복사하기

	char stringD[14]{ "Hello, World!" };
	char stringE[14]{};


	// 기존 방식 (for문 이용)
	for (int i = 0; i < 14; i++)
	{
		stringE[i] = stringD[i];
	}

	// string copy (safe) 이용 - 기존 문자열, 크기, 복사할 문자열을 입력받는다. 복사된 문자열의 포인터를 반환한다.
	//	* safe를 쓰는 이유는, 문자열의 맨 마지막엔 문자열을 끝낸다는 의미에서 자동으로 null 문자('\0')가 들어가는데, 이 자리에 문자열을 복사하면 출력 시 문자열 끝을 알 수 없어 예기치 못한 출력을 발생시키기 때문이다.
	strcpy_s(stringD, 14, stringE);

}