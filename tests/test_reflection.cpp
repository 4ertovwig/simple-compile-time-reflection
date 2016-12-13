#include <../funny_reflection/reflection.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>

//тесты

BOOST_AUTO_TEST_SUITE(ReflectionTests)

class test_struct {
public:
    test_struct(int a, double b, float c, unsigned long long d)
        : a(a), b(b), c(c), d(d)
    {
    }
private:
    int a;
    const double b;
    float c;
    unsigned long long d;

    int foo(int&, double&&) {
        return 1;
    }

    int foo(int&, double&&) const {
        return 2;
    }

    int foo(int&, double&) {
        return 3;
    }

    REFLECTABLE_FIELDS(
        ((a, 3))
        ((b, 4))
        ((c, 8))
        ((d, 23))
    )
    REFLECTABLE_FUNCTIONS(
        ((int(int&,double&&), foo, 1))
        ((int(int&,double&&) const, foo, 2))
        ((int(int&,double&), foo, 3))
    )
};

//using namespace equations_nsp;

//сумма полей структуры
BOOST_AUTO_TEST_CASE(sum_of_fields) {
    const test_struct str = { 1, 2., 3.f, 4 };
    const int result = 10;
    int sum = 0;
    refl::for_each_fields(str, [&](auto&& el) { sum+=el; });

    BOOST_REQUIRE_EQUAL(result, sum);
}

BOOST_AUTO_TEST_CASE(some_fields) {
    const test_struct str = { 1, 2., 3.f, 4 };

    BOOST_CHECK((refl::return_field<3>(str) == 1) &&
                (std::is_same<refl::field_type<3, decltype(str)>::type, int>::value));

    BOOST_CHECK((refl::return_field<4>(str) == 2.) &&
                (std::is_same<refl::field_type<4, decltype(str)>::type, double>::value));

    BOOST_CHECK((refl::return_field<8>(str) == 3.f) &&
                (std::is_same<refl::field_type<8, decltype(str)>::type, float>::value));

    BOOST_CHECK((refl::return_field<23>(str) == 4) &&
                (std::is_same<refl::field_type<23, decltype(str)>::type, unsigned long long>::value));
}

BOOST_AUTO_TEST_CASE(invoke_some_functions) {
    test_struct str = { 1, 2., 3.f, 4 };

    int a1 = 1;
    BOOST_REQUIRE_EQUAL(refl::invoke_func<1>(str, a1, 2.), 1);

    BOOST_REQUIRE_EQUAL(refl::invoke_func<2>(str, a1, 2.), 2);

    double a2 = 1.;
    BOOST_REQUIRE_EQUAL(refl::invoke_func<3>(str, a1, a2), 3);
}

BOOST_AUTO_TEST_SUITE_END()
