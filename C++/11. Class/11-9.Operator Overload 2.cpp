#include <iostream>

class Point2D
{
public:
	int mX{};
	int mY{};

public:
	Point2D(int x, int y) : mX{ x }, mY{ y } {}
	void Print()
	{
		std::cout << "(" << mX << ", " << mY << ")" << std::endl;
	}

	// 반환형이 Point2D라면, 반환형을 저장할 Point2D 형태의 값이 하나 더 생겨난다. 나 자신의 값을 바꾸는데 내부적으로 복사가 한번 더 일어나는 셈.
	// 이를 방지하기 위해 참조형인 Point2D&를 쓴다.
	// 연산자 오버로딩에는 const를 붙이는 게 좋다. 그렇지 않으면 이 인스턴스의 참조를 Point2D&로 받아 멤버 변수를 맘대로 변경할 수 있기 때문이다.
	// 이 사례에 대해서는 main 함수 참조
	Point2D& operator ++ ()
	{
		++mX;
		++mY;
		return *this; //이 인스턴스인 this를 넘기되, 클래스 자체는 포인터형 변수인데 값을 넘겨야 하므로 *를 붙인다.
	}

	// 후위 연산자는 뒤에 다른 요소가 있는 것처럼 임시 매개변수를 넣어 구분한다.
	const Point2D& operator ++ (int)
	{
		// 값을 증가시키되 증가 전의 값을 반환해야 하므로, 임시 변수가 필요하다.
		// 나 자신을 복사하려면 복사 생성자를 이용한다.
		Point2D temp(*this);

		// mX,mY 값의 증가는 위에 만들어둔 증가 연산자 ++를 활용한다.
		++(*this);

		return temp;
	}

	// 이항 연산자는 나 자신을 변경시키기보다 다른 곳에 대입되는 형태로 쓰이므로, 어차피 다른 곳에 복사가 일어난다.
	// 따라서 굳이 참조형으로 반환될 필요가 없다. 참조형이 아니니 const도 붙일 필요가 없다.
	Point2D operator + (Point2D other)
	{
		return Point2D(mX + other.mX, mY + other.mY);
	}
	Point2D operator - (Point2D other)
	{
		return Point2D(mX - other.mX, mY - other.mY);
	}

	// 형 변환 연산자는 반환형을 입력하지 않아도, 형태로 추측이 가능하므로 적는다.
	operator float()
	{
		return sqrt(mX * mX + mY + mY);
	}

	// 이 클래스에 소속되어 있지 않은 std::ostream 클래스를 매개변수로 연산하려면, 이 함수의 멤버 변수에 접근할 수 있게 해야 한다.
	// friend를 붙이면 클래스의 캡슐화가 깨진다. private 변수도 접근 가능하다.
	// 캡슐화를 깨기 때문에 연산자 오버로딩 외에는 일반적으로 잘 쓰이진 않는다. friend에 대해서는 추후 다시 설명할 예정.
	friend std::ostream& operator << (std::ostream& os, const Point2D& pt)
	{
		os << "(" << pt.mX << ", " << pt.mY << ")";
		return os;
	}
};

int main()
{
	// 연산자 오버로딩과 반환형
	
	Point2D p1(1, 1);
	Point2D p2(0, 0);
	Point2D p3(1, 3);

	// Point2D&인 경우 함수를 호출할 p1의 값을 바로 바꾸고 이를 반환하므로, 복사는 p2에 값을 옮길 때 1번만 하면 된다.
	p2 = p1 + p3;
	p2.Print();

	// 연산자 오버로딩에 반환형이 참조형(Point2D&)인데 const를 붙이지 않을 경우, 아래와 같은 문제가 생길 수 있다.
	Point2D& temp = ++p1;
	temp.mX = 10;
	// 연산 결과를 참조형으로 받은 다음, 멤버 변수 값 mX에 맘대로 2를 대입했다.

	p1.Print();
	// ++p1을 한 시점에서 p1의 값은 (2,2) 가 되지만, mX에 10을 대입했으니 (10,2)가 될 것이다.


	std::cout << "---------------" << std::endl;
	//--------------------------------------------------------------

	// 전위(prefix), 후위(postfix) 연산자

	Point2D p4(0, 0);
	Point2D p5(0, 0);
	Point2D p6(0, 0);
	Point2D p7(0, 0);

	p5 = ++p4;
	p7 = p6++;

	p5.Print(); // 전위 연산자를 썼으므로 값이 증가해 있다.
	p7.Print(); // 후위 연산자를 썼으므로 값이 증가하기 전에 대입되었다.

	std::cout << "---------------" << std::endl;
	//--------------------------------------------------------------

	// 형 변환 연산자
	Point2D p8(2, 3);

	// (float)로 형변환하면, 위에서 정의한 것과 같이 벡터의 길이가 반환된다. float 값처럼 사용할 수 있다.
	double dist = (float)p8;

	// 형변환한 값을 로그로 찍는 것도 가능.
	std::cout << "벡터 길이: " << (float)p8 << std::endl;

	// std::ostream의 << 연산자를 오버로드하면, 원하는 방식의 출력이 가능하다.
	std::cout << p8 << std::endl;

}