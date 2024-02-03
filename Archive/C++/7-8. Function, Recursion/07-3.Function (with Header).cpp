#include <iostream> // C++의 기본 기능을 가져올때는 부등호 기호 <> 를 사용한다.
#include "TestHeader.h" //선언을 가져오는 전처리기. 헤더 파일명을 명시한다.

int main()
{
    int index{};

    std::cout << "몇 단을 출력할까요? : ";
    std::cin >> index;

    GuGuDan(index);

    int a{}, b{};
    std::cout << "나눌 숫자 a,b를 입력하세요. (입력 예시: 17 4)" << std::endl << ">> ";
    std::cin >> a >> b;

    std::cout << a << "를 " << b << "로 나눈 몫은 " << Quotient(a, b) <<
        ", 나머지는 " << Remainder(a, b) << " 입니다." << std::endl;
}