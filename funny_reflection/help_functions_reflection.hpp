/******************************************************************************
     * File: help_functions_reflection.hpp
     * Description: реализация функций для рефлексии
     * Created: 10 oct 2016
     * Author: Ilya Korotkov
     * Email: lord2402@mail.ru
******************************************************************************/
#ifndef HELP_FUNCTIONS_REFLECTION_HPP
#define HELP_FUNCTIONS_REFLECTION_HPP

namespace refl {

    /**
     * метод возвращает поле рефлексируемой сущности
     * по указанному индексу
     * @param obj - объект рефлексируемой сущности
     * @return - возвращаем поле
    */
    template<std::size_t index,
             typename T>
    decltype(auto) return_field(T&& obj)
    {
        return typename std::remove_reference_t<T>::template ret<T,index>(std::forward<T>(obj))();
    }

    /**
     * метафункция "возвращающая" тип поля рефлексирующей
     * сущности по указанному индексу
    */
    template<std::size_t index,
             typename T>
    struct field_type {
        using type = decltype(return_field<index, T>(std::declval<T>()));
    };

    namespace aux {

        ///рекурсивно вызываем функциональный объект F&& f для каждого поля
        template<typename T,
                 typename seq = typename std::remove_reference_t<T>::indexes,
                 size_t I = boost::mpl::size<seq>::type::value-1>
        struct for_each_fields_impl {
            template<typename F,
                     typename TT>
            void operator()(F&& f, TT&& z) {
                for_each_fields_impl<T, typename boost::mpl::pop_back<seq>::type, I-1> f_;
                f_(std::forward<F>(f), std::forward<TT>(z));
                f(return_field<boost::mpl::back<seq>::type::value, T>(std::forward<TT>(z)));
            }
        };
        ///остановка рекурсии
        template<typename T,
                 typename seq>
        struct for_each_fields_impl<T, seq, 0> {
            template<typename F,
                     typename TT>
            void operator()(F&& f, TT&& z) {
                f(return_field<boost::mpl::back<seq>::type::value, T>(std::forward<TT>(z)));
            }
        };

    }   //namespace aux

    /**
     * функция высшего порядка применяющая указанный
     * функтор для каждого поля рефлексируемой сущности
     * иммутабельна т.е. не изменяет сами поля
     * @param obj - объект рефлексируемой сущности
     * @param functor - функциональный объект
    */
    template<typename T,
             typename F>
    decltype(auto) for_each_fields(T&& obj, F&& functor) {
        aux::for_each_fields_impl<T> f_;
        f_(std::forward<F>(functor), std::forward<T>(obj));
    }

    /**
     * функция высшего порядка для каждого индекса
     * поля рефлексируемой сущности
     * иммутабельна т.е. не изменяет сами индексы
     * @param functor - функциональный объект
    */
    template<typename T,
             typename F>
    decltype(auto) for_each_index(T&&, F&& f) {
        boost::mpl::for_each<typename std::remove_reference_t<T>::indexes>
                        (std::forward<F>(f));
    }

    /**
     * функция-обертка для вызова функции-члена
     * по указанному индексу в рефлексируемой сущности
     * @param obj - объект рефлексируемой сущности
     * @param args - аргументы вызываемой функции
    */
    template<std::size_t index,
             typename T,
             typename...Arg>
    decltype(auto) invoke_func(T&& obj, Arg&&...args) {
        return typename std::remove_reference_t<T>::template func_ret<index, T, Arg...>
                                            (std::forward<T>(obj))(std::forward<Arg>(args)...);
    }

}   //namespace refl

#endif // HELP_FUNCTIONS_REFLECTION_HPP
