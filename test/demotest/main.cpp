#include "mu_test.h"
#include <climits>
#include <vector>
#include<iostream>
#include<iomanip>
#include<cmath>

using namespace std;

double pi(size_t n) {
    double sum = 0.0;
    int sign = 1;
    for (size_t i = 0; i < n; ++i) {
        sum += sign/(2.0*i+1.0);
        sign *= -1;
    }
    return 4.0*sum;
}

BEGIN_TEST(pi_raw_short_test)
    TRACER << fixed << setprecision(10);
    double pi_1000 = pi(1000);
    TRACER << "\tdemo test: " << pi_1000 << '\n';

    double delta = M_PI - pi_1000;
    TRACE(pi_1000);
    TRACE(delta);

    ASSERT_THAT(std::abs(pi_1000-M_PI) < 1e-2);
END_TEST


BEGIN_TEST(pi_raw_long_test)
    double p = pi(100000);
    TRACER << fixed << setprecision(10);
    TRACER << "\t\ttaylor: " << p << " delta: " << M_PI - p << '\n';
    ASSERT_THAT(std::abs(p-M_PI) < 1e-4);
    TRACE(std::abs(p-M_PI) < 1e-4);
END_TEST



BEGIN_TEST(pi_raw_very_long_test)
    double p = pi(500000);
    // TRACER << fixed << setprecision(10);
    // TRACER << "\t\ttaylor: " << p << " delta: " << M_PI - p << '\n';
    ASSERT_THAT(std::abs(p-M_PI) < 1e-4);
END_TEST

BEGIN_TEST(pi_raw_ultra_long_test)
    double p = pi(2000000);
    // TRACER << fixed << setprecision(10);
    // TRACER << "\t\ttaylor: " << p << " delta: " << M_PI - p << '\n';
    ASSERT_THAT(std::abs(p-M_PI) < 1e-4);
END_TEST

BEGIN_TEST(string_operations)
    std::string s = "ab";
    std::string t = "cd";

    ASSERT_EQUAL(s+t, "abcd");
    ASSERT_EQUAL(t+s, "cdab");

    ASSERT_THAT(s+t != t+s);
    ASSERT_THAT(s < t);
END_TEST

BEGIN_TEST(add_ints_big)
    int a = INT_MAX / 2;
    ++a;
    int b = INT_MAX / 2;
    ++b;

    long r = a + b;
    TRACE(r);
    ASSERT_THAT(r > 0);
    TRACE(r > 0);
END_TEST

BEGIN_TEST(panic)
    int* p = 0;
    *p = 42;

    ASSERT_FAIL("what? we managed to dereference a null pointer???");
END_TEST

std::vector<int> get_zero() { return std::vector<int>(1); }
BEGIN_TEST(div_by_zero_should_signal)
    int a = 42;
    std::vector<int> v = get_zero();
    v.pop_back();

    double infinity = a/v.size(); // SIGFPE
    static_cast<void>(infinity);
    ASSERT_FAIL("what? we managed to divide by zero???");
END_TEST

void checkPassword(const char* ){
    throw std::runtime_error("wrong password!");
}

BEGIN_TEST(using_wrong_password_should_throw)
    try{
        checkPassword("--wrong--");
        ASSERT_FAIL("checkPassword failed to throw exception on wrong password!");
    }catch(std::runtime_error const& x){
        ASSERT_EQUAL(x.what(), std::string("wrong password!"));
    }
END_TEST


BEGIN_TEST(test_handling_bad_casts)
    std::istream& is = dynamic_cast<std::istream&>(std::cout);
    static_cast<void>(is);
END_TEST

BEGIN_TEST(double_div_by_zero_test)
    double a = 4;
    double b = 0;

    double r = a / b;
    a = 1 / r;
    TRACE(a);
    ASSERT_THAT(r > 0);
END_TEST


BEGIN_TEST(demo)
    const char* s = "hello";
    const char* q = "world";
    ASSERT_THAT(s);
    ASSERT_THAT(s != q);

    ASSERT_EQUAL(s[4], 'o');
    ASSERT_NOT_EQUAL(s[1], q[1]);

    // ASSERT_EQUAL(s, q);
    {
        std::string s("C++");
        std::string q("Java");
        ASSERT_EQUAL(s[0], 'C');
        ASSERT_NOT_EQUAL(s, q);
        // ASSERT_NOT_EQUAL(s.begin(), s.end()); // will not compile
    }

END_TEST

void will_not_throw() {
    throw 666;
}

BEGIN_TEST(test_pass)
    try{
        will_not_throw();
        ASSERT_PASS();
    }catch(...){
        ASSERT_FAIL("was not supposed to throw");
    }
END_TEST

BEGIN_SUITE(不耻下问)
    TEST(string_operations)

    IGNORE_TEST(add_ints_big)

    TEST(pi_raw_short_test)
    TEST(pi_raw_long_test)
    TEST(pi_raw_very_long_test)
    TEST(pi_raw_ultra_long_test)

    TEST(panic)
    TEST(div_by_zero_should_signal)

    TEST(using_wrong_password_should_throw)
    TEST(test_handling_bad_casts)

    TEST(double_div_by_zero_test)
    TEST(demo)
    TEST(test_pass)
END_SUITE
