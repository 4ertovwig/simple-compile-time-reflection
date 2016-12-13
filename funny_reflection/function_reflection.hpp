/******************************************************************************
     * File: function_reflection.hpp
     * Description: реализация макросов для рефлексии функций
     * Created: 10 oct 2016
     * Author: Ilya Korotkov
     * Email: lord2402@mail.ru
******************************************************************************/
#ifndef FUNCTION_REFLECTION_HPP
#define FUNCTION_REFLECTION_HPP
#
#
#
///эта простыня генерирует внутренние структуры с доступом
/// к функциям рефлексируемого типа
#ifndef REFLECTABLE_FUNCTION
#define REFLECTABLE_FUNCTION(tuple)                                         \
    template<typename T, typename...Arg>                                    \
    struct func_ret<BOOST_PP_TUPLE_ELEM(                                    \
                        BOOST_PP_TUPLE_SIZE(tuple),2,tuple),                \
                    T,                                                      \
                    Arg...>                                                 \
    {                                                                       \
        template<typename T_c>                                              \
        func_ret(T_c&& own)                                                 \
            : owner(std::forward<T_c>(own)),                                \
              ptr(&std::remove_reference_t<T_c>                             \
                            ::BOOST_PP_TUPLE_ELEM(                          \
                              BOOST_PP_TUPLE_SIZE(tuple),1,tuple)           \
                 )                                                          \
        {                                                                   \
        }                                                                   \
        template<typename...TT>                                             \
        auto operator()(TT&&...args) {                                      \
            return ptr(&owner, std::forward<TT>(args)...);                  \
        }                                                                   \
    private:                                                                \
        decltype(std::mem_fn<BOOST_PP_TUPLE_ELEM(                           \
                                BOOST_PP_TUPLE_SIZE(tuple),0,tuple)         \
                            >(&std::remove_reference_t<T>::                 \
                                BOOST_PP_TUPLE_ELEM(                        \
                                    BOOST_PP_TUPLE_SIZE(tuple),1,tuple))    \
                ) ptr;                                                      \
        T owner;                                                            \
    };
#
#endif
#
/**
 * макрос выше превращается в это:
 *
#ifndef REFLECTABLE_FUNCTION
#define REFLECTABLE_FUNCTION((signature, name, index))          \
    template<typename T, typename...Arg>                        \
    struct func_ret<index, T, Arg...> {                         \
        template<typename T_c>                                  \
        func_ret(T_c&& own)                                     \
            : owner(std::forward<T_c>(own)),                    \
              ptr(&std::remove_reference_t<T_c>::name)          \
        {                                                       \
        }                                                       \
        template<typename...TT>                                 \
        auto operator()(TT&&...args) {                          \
            return ptr(&owner, std::forward<TT>(args)...);      \
        }                                                       \
    private:                                                    \
        decltype(std::mem_fn<signature>(                        \
                            &std::remove_reference_t<T>::name)  \
                ) ptr;                                          \
        T owner;                                                \
    };
#
#endif */
#
#
///генерация объявления функтора вызывающих функции-члены
#ifndef GENERATE_REFLECTABLE_FUNCTION_HEAD
#define GENERATE_REFLECTABLE_FUNCTION_HEAD  \
            template<std::size_t I,         \
                     typename...Arg>        \
            struct func_ret;
#endif
#
#
///дружественная функция для вызова функций-членов
#ifndef FRIEND_INVOKE_FUNCTION
#define FRIEND_INVOKE_FUNCTION                                          \
            template<std::size_t index,                                 \
                     typename T,                                        \
                     typename...Arg>                                    \
            friend decltype(auto) refl::invoke_func( T&&, Arg&&... );
#endif
#
#
#
#ifndef GENERATE_REFLECTABLE_FUNCTORS_INVOKE_FUNCTION
#define GENERATE_REFLECTABLE_FUNCTORS_INVOKE_FUNCTION(a,b,tuple)     \
                   REFLECTABLE_FUNCTION(tuple)
#endif
#
#
#ifndef REFLECTABLE_FUNCTIONS_I
#define REFLECTABLE_FUNCTIONS_I(sequence)                            \
            GENERATE_REFLECTABLE_FUNCTION_HEAD                       \
            BOOST_PP_SEQ_FOR_EACH(                                   \
                    GENERATE_REFLECTABLE_FUNCTORS_INVOKE_FUNCTION,   \
                    BOOST_PP_EMPTY,                                  \
                    sequence);
#endif
#
#
///итоговый макрос для рефлексии функций
#ifndef REFLECTABLE_FUNCTIONS
#define REFLECTABLE_FUNCTIONS(sequence)         \
            FRIEND_INVOKE_FUNCTION              \
            REFLECTABLE_FUNCTIONS_I(sequence)
#endif
#
#
#endif // FUNCTION_REFLECTION_HPP
