#include <iostream>
#include <fstream>

// 파일 읽기 - include fstream(파일 스트림) 필요

// 1. 파일 스트림 객체 생성 (ifstream)
// 2. 파일 읽기
// 3. 파일 닫기
// 입출력은 항상 이 절차를 따르게 된다.

bool LoadFile(const char* filename)
{
	// 스트림 객체를 생성한다.
	std::ifstream ist(filename);

	// 기본적으로 ifstream은 텍스트 파일의 맨 앞 위치를 불러오지만, 두 번째 인자를 추가하여 다른 방식으로 불러올 수있다.
	std::ifstream stream1(filename, std::ifstream::ate); // at end의 약자로, 파일의 맨 뒤 위치를 읽는다.
	std::ifstream stream3(filename, std::ifstream::ate | std::ifstream::binary); // 여러 개 병렬로 사용하려면 |(or)을 사용한다. binary는 이진수로 읽어온다.

	ist.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	if (!ist) // 파일이 없을 경우
	{
		// std::cerr(C에러)은 cout과 비슷한 출력을 주지만, 나중에 에러 메시지를 구분할 수 있다.
		std::cerr << filename << "을 찾을 수 없습니다." << std::endl;
		return false;
	}

	// 에러 비트 (Error bit)
	// 예외 처리를 위해, ifstream은 항상 자신의 상태를 저장해 놓는다.
	// goodbit : 정상일 때 켜짐
	// eofbit : 파일의 끝에서 연산을 시도하면 켜짐
	// failbit : 논리적 오류가 있을 때 켜짐
	// badbit : 심각한 입/출력 오류가 있을 때 켜짐

	// 위의 비트는 아래의 함수로 불러와 확인할 수 있다.
	// good() : goodbit가 켜져있을 경우에만 true
	// eof() : eofbit가 켜져있을 경우에만 true
	// fail() : failbit 또는 badbit가 켜져있으면 true
	// bad() : badbit가 켜져있을 경우에만 true

	// 스트림 객체에서 파일을 한 글자씩 가져와서 출력한다.
	// 기본적으로 엔터키를 포함한 특수문자는 가져오지 않는데, 특수문자까지 모두 읽어오려면 ist.get()과 같이 쓰면 된다.
	char ch;
	while (ist >> ch)
	{
		if (ist.bad())
		{
			std::cerr << "파일이 손상되었습니다." << std::endl;
			ist.close();
			return false;
		}
		if (ist.fail())
		{
			std::cerr << "잘못된 포맷입니다." << std::endl;
			ist.close();
			return false;
		}

		std::cout << ch;
	}


	// 파일을 닫는다.
	ist.close();

	return true;
}

int main()
{
	LoadFile("SimpleData.txt");
	// 현재 위치에 파일이 있을 경우 파일명만, 하위 폴더에 있을 경우 "폴더명/폴더명/ ... /파일명"과 같이 쓴다.
	//	ㄴ 예시: "Data/SimpleData.txt"

	// 참고로 folder / file (슬래시 기호로 하위 폴더 접근) 형태는 유닉스, 리눅스, 맥OS의 표준이다.
	// 윈도우 표준은 folder \ file (역슬래시 기호로 하위 폴더 접근) 형태이다.
	// 어떻게 쓰든 컴파일러가 알아서 처리해주지만, 윈도우 전용 프로그램에서는 역슬래시를 사용해야 함에 유의.
}