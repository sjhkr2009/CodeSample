#include <iostream>

// static 변수의 활용

// 쿠폰을 생성하는 클래스를 가정한다.
class CouponGenerator
{
private:
	int count;
public:
	CouponGenerator() : count{ 0 } {}
	void CreateNewCoupon() { std::cout << ++count << "번 쿠폰 발행" << std::endl; };
};

class CouponGenerator2
{
private:
	static int count;
public:
	void CreateNewCoupon() { std::cout << ++count << "번 쿠폰 발행" << std::endl; };
};

class CouponGenerator3
{
private:
	static int count;
public:
	static void CreateNewCoupon() { std::cout << ++count << "번 쿠폰 발행" << std::endl; };
};

// static 변수는 클래스 외부에서 초기화해야 한다.
int CouponGenerator2::count{ 0 };
int CouponGenerator3::count{ 0 };

int main()
{
	CouponGenerator g1;

	g1.CreateNewCoupon(); // 쿠폰은 중복되면 안 되므로, 생성할 때마다 값을 바꿔줄 수 있다.
	g1.CreateNewCoupon();

	CouponGenerator g2;

	g2.CreateNewCoupon(); // 하지만 생성기가 여러 개라면, 멤버 변수/함수는 인스턴스 종속이므로 값이 초기화된다.
	g2.CreateNewCoupon();
	g2.CreateNewCoupon();

	std::cout << "---------------------" << std::endl;

	CouponGenerator2 g3;

	g3.CreateNewCoupon();
	g3.CreateNewCoupon();
	g3.CreateNewCoupon();

	CouponGenerator2 g4;

	g4.CreateNewCoupon(); // 정적 변수를 활용하면 인스턴스를 여러 개 만들어도 값이 초기화되지 않는다.
	g4.CreateNewCoupon();
	g4.CreateNewCoupon();

	std::cout << "---------------------" << std::endl;

	CouponGenerator3::CreateNewCoupon(); // 정적 함수는 인스턴스를 생성하지 않고도 사용할 수 있다.
	CouponGenerator3::CreateNewCoupon();
	CouponGenerator3::CreateNewCoupon();
	CouponGenerator3::CreateNewCoupon();
	CouponGenerator3::CreateNewCoupon();
}

// 실제로 Input과 같이 인스턴스에 종속될 필요가 없는 부분의 경우,
// Mouse::GetButton() 과 같이 static 형태로 사용한다.