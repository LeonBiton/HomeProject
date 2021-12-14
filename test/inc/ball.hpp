#include <iostream>
#include <string>


class TestBall{
public:
    TestBall(double a_radius , std::string a_color);
    TestBall();
    ~TestBall() = default;

    TestBall& operator=(const TestBall &a_testBall);

    bool operator<(TestBall const & a_testBall)const;
    bool operator==(TestBall const & a_testBal)const;

    double GetRadius()const{return m_radius;}

private:
    double m_radius;
    std::string m_color;
};