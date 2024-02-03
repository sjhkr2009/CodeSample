#include <iostream>
#include <fstream>

// 파일 읽기 - 예외 처리

bool LoadFile(const char* filename)
{
	std::ifstream ist;

	// 예외 처리는 if문으로 bad(), fail()을 체크하는 대신, 아래와 같이 표현할 수 있다.
	// failbit 또는 badbit가 켜지면 예외를 일으킴을 의미한다.
	ist.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try
	{
		ist.open(filename);

		char ch;
		while (ist >> ch)
		{
			std::cout << ch;
		}

		ist.close();
	}
	catch (std::ifstream::failure e)
	{
		if (ist.eof())
		{
			// 파일 끝에서 더 읽으려 해도 논리적 오류인 failbit가 켜지므로, 이 경우는 정상적으로 읽었으니 종료시킨다.
			std::cerr << std::endl << "파일 읽기를 완료했습니다." << std::endl;
		}
		else
		{
			std::cerr << std::endl << "파일을 읽는 도중 예외가 발생했습니다 : " <<
				e.what() << std::endl;
		}

		ist.close();
		return false;
	}

	return true;
}

int main()
{
	LoadFile("SimpleData.txt");
}