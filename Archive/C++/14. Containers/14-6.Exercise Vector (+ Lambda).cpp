#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

// 학생들의 점수 출력하기
// 인터넷 검색 활용

class Student
{
public:
	int			mNumber;	// 학생 번호
	std::string	mName;		// 이름
	int			mScore;		// 점수

	void Print()
	{
		std::cout << mNumber << " - " << mName << "'s Score: " << mScore << "점" << std::endl;
	}

public:
	Student(int number, std::string name, int score) :
		mNumber{ number }, mName{ name }, mScore{ score }
	{}
};

int main()
{
	//선언 및 정의
	std::vector<Student> students{
		{1, "Kim", 80},
		{2, "Lee", 20},
		{3, "Park", 50},
		{4, "Choi", 30}
	};

	// 추가
	students.push_back(Student(5, "Hwang", 60));

	// 순회하며 출력
	std::for_each(students.begin(), students.end(),
		[](Student& stu) { stu.Print(); }
		);

	// 특정 조건의 원소 찾기
	// 람다식 내 매개변수도 auto로 사용할 수 있다.
	std::find_if(students.begin(), students.end(),
		[](auto& stu) {return (stu.mNumber == 3); }
	)
		->Print();

	// 합계 및 평균 구하기
	int sum{};
	float average{};

	sum = std::accumulate(students.begin(), students.end(), 0,
		[](int _sum, Student& stu) { return _sum += stu.mScore; }
		);

	average = static_cast<float>(sum) / students.size();

	std::cout << "반 총점: " << sum << "점" << std::endl <<
		"반 평균: " << average << "점" << std::endl;
}
