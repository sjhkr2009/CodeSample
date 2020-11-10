#include <iostream>
#include <map>
#include <numeric>

// MAP

class Student
{
public:
	int			mNumber;
	std::string	mName;
	int			mScore;

	void Print(bool includeNumber = false)
	{
		if (includeNumber)
		{
			std::cout << mNumber << ". ";
		}
		std::cout << mName << " : " << mScore << "점" << std::endl;
	}

public:
	Student(int number, std::string name, int score) :
		mNumber{ number }, mName { name }, mScore{ score }
	{}
};

using StuDict = std::pair<int, Student>;

int main()
{
	std::map<int, Student> students{
		{1, Student(1, "Kim", 80)},
		{2, Student(2, "Lee", 20)},
		{3, Student(3, "Park", 50)},
		{4, Student(4, "Choi", 30)}
	};

	students.insert(StuDict(5, Student(5, "Hwang", 60)));

	for (auto& student : students)
	{
		std::cout << student.first << ". ";
		student.second.Print();
	}

	int sum{
		std::accumulate(students.begin(), students.end(), 0,
			[](int _sum, StuDict& pair)
			{
				return _sum += pair.second.mScore;
			}
		)
	};
	float average{ static_cast<float>(sum) / students.size() };

	std::cout << "반 총점: " << sum << "점" << std::endl <<
		"반 평균: " << average << "점" << std::endl;
}