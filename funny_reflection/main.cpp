#include <iostream>

#include <reflection.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/max_element.hpp>

//пара примеров использования

struct example1 {

    int a;
    double b;
    std::string c;

    REFLECTABLE_FIELDS(
        ((a, 1))
        ((b, 5))
        ((c, 18))
        )
};

class example2 : example1 {
public:
    template<typename T1,
             typename T2,
             typename T3,
             typename T4>
    example2(T1&& t1, T2&& t2, T3&& t3, T4&& t4) {
        a = std::forward<T1>(t1);
        b = std::forward<T2>(t2);
        c = std::forward<T3>(t3);
        d = std::forward<T4>(t4);
    }
private:
    float d;
public:
    REFLECTABLE_FIELDS(
        ((a, 1))
        ((b, 2))
        ((c, 3))
        ((d, 4))
        )
};

struct example3_ {
    example3_(std::string a_, double b_)
        : a(a_), b(b_) {}
private:
    std::string a;
    double b;
};

struct example3 : example3_ {
    example3(std::string a_,
             double b_,
             float c_,
             int d_)
        : example3_(a_, b_), c(c_), d(d_)
    {}
private:
    float c;
    int d;
public:
    REFLECTABLE_FIELDS(
        ((a, 25))
        ((b, 22))
        ((c, 41))
        ((d, 400))
        )
};

class example4_ {
public:
    float parent_func(double&, std::string&& str) {
        std::cout<<str<<std::endl;
        return 1.f;
    }
};

//можно определить пользовательскую функцию
//находящую минимальный и максимальный индексы полей
//рефлексируемой структуры
//но в случае новой пользовательской функции, чтобы
//она могла работать, макрос reflectable_fields
//должен быть в секции public

namespace refl {

template<typename T>
std::pair<int, int> min_max(const T& t);

namespace aux {
    template<typename T,
             typename seq = typename std::remove_reference_t<T>::indexes>
    struct min_max_impl {
        template<typename TT>
        friend std::pair<int, int> refl::min_max(const TT& t);

        void operator()(const T&) {
            min_value = boost::mpl::min_element<seq>::type::type::value;
            max_value = boost::mpl::max_element<seq>::type::type::value;
        }
    private:
        int min_value;
        int max_value;
    };
}   //namespace aux

template<typename T>
std::pair<int, int> min_max(const T& t) {
    aux::min_max_impl<T> f_; f_(t);
    return std::make_pair(f_.min_value, f_.max_value);
}

}   //namespace refl


class example4 : public example4_ {
    void hi() {
        std::cout<<"hi\n";
    }

    void hi() const {
        std::cout<<"hi const\n";
    }

    double sum(const double& a, const double& b) {
        std::cout<<"sum double " << a + b << "\n";
        return a+b;
    }
    double sum(int& a, int& b, int& c) const {
        std::cout<<"sum int " << a + b + c << "\n";
        return a+b;
    }
    double sum(double&& a, double& b) {
        std::cout<<"sum double\n";
        return a+b;
    }
    double sum(double&& a, const double& b) const {
        std::cout<<"const sum const double\n";
        return a+b;
    }

    void hippie(int&&) const {
        std::cout<<"hippie\n";
    }
public:
    REFLECTABLE_FUNCTIONS(
        ((void(), hi, 1))
        ((void() const, hi, 2))
        ((double(const double&, const double&), sum, 3))
        ((double(int&, int&, int&) const, sum, 4))
        ((double(double&&, double&),sum, 5))
        ((double(double&&, const double&) const, sum, 6))
        ((void(int&&) const, hippie, 7))
        ((float(double&, std::string&&), parent_func, 8))
    )
};

int main(int, char *[])
{
    example1 t1 = { 1, 2., "hello world"};

    //выводим поле с индексом 18
    std::cout<<refl::return_field<18>(t1)<<std::endl;
    //тип поля по индексу
    std::cout<<std::boolalpha;
    std::cout<<"type of field at index 5 is double? - "
             <<std::is_same<
                        refl::field_type<5, example1>::type, double
                           >::value
             <<std::endl;
    std::cout<<"type of field at index 18 is std::string? - "
             <<std::is_same<
                        refl::field_type<18, example1>::type, std::string
                           >::value
             <<std::endl;
    std::cout<<std::noboolalpha;
    //выводим все поля на экран
    refl::for_each_fields(t1, [](auto&& el) { std::cout<<el<<" ";});
    std::cout<<std::endl;
    //выводим все индексы на экран
    refl::for_each_index(t1, [](auto&& el) { std::cout<<el<<" ";});
    std::cout<<std::endl;

    example2 t2 ( 1, 1., "1", 1.f );
    refl::for_each_fields(t2, [](auto&& el) { std::cout<<el<<" ";});
    std::cout<<std::endl;
    //минимальный и максимальный индексы полей
    example3 t3 ("1", 1., 1.f, 1 );
    std::cout<<"minimal index :"<<refl::min_max(t3).first<<"\n"
             <<"maximal index :"<<refl::min_max(t3).second<<"\n";
    //вызываем функции по индексам
    example4 t4;
    refl::invoke_func<1>(t4);
    refl::invoke_func<2>(t4);
    refl::invoke_func<3>(t4, 2., 2.);
    int a = 1,b = 1,c = 1;
    refl::invoke_func<4>(t4, a, b, c);
    double d = 6.;
    refl::invoke_func<5>(t4, 8., d);
    refl::invoke_func<6>(t4 , 15., 15.);
    refl::invoke_func<7>(t4, 1);
    refl::invoke_func<8>(t4, d, "bye, funny reflection");

    return 0;
}
