/******************************************************************************
     * File: fields_reflection.hpp
     * Description: реализация макросов для рефлексии полей
     * Created: 10 oct 2016
     * Author: Ilya Korotkov
     * Email: lord2402@mail.ru
******************************************************************************/
#ifndef FIELDS_REFLECTION_HPP
#define FIELDS_REFLECTION_HPP
#
#
#
///генерация объявления функтора возвращаемых значений
#ifndef GENERATE_HEAD_FUNCTOR
#define GENERATE_HEAD_FUNCTOR                               \
            template<typename T, std::size_t i>             \
            struct ret;
#endif
#
#
///эта простыня генерирует внутренние структуры с доступом
/// к полям рефлексируемого типа
#
#ifndef GENERATE_FUNCTOR
#define GENERATE_FUNCTOR(tuple)                                     \
    template<typename T>                                            \
    struct ret<T,BOOST_PP_TUPLE_ELEM(                               \
                        BOOST_PP_TUPLE_SIZE(tuple),1,tuple)>        \
    {                                                               \
                                                                    \
        ret(const T& owner)                                         \
            :m(owner)                                               \
        {                                                           \
        }                                                           \
        auto operator()()                                           \
        {                                                           \
            return m.BOOST_PP_TUPLE_ELEM(                           \
                        BOOST_PP_TUPLE_SIZE(tuple),0,tuple          \
                                      );                            \
        }                                                           \
    private:                                                        \
        T m;                                                        \
    };
#endif
#
#
#
#ifndef GENERATE_REFLECTABLE_FUNCTORS_INVOKE
#define GENERATE_REFLECTABLE_FUNCTORS_INVOKE(a,b,tuple)     \
                   GENERATE_FUNCTOR(tuple)
#
#endif
#
#
#
#ifndef REFLECTABLE_FIELDS_I
#define REFLECTABLE_FIELDS_I(sequence)                      \
            GENERATE_HEAD_FUNCTOR                           \
            BOOST_PP_SEQ_FOR_EACH(                          \
                    GENERATE_REFLECTABLE_FUNCTORS_INVOKE,   \
                    BOOST_PP_EMPTY,                         \
                    sequence);
#endif
#
#
///вспомогательный макрос для генерации списка индексов
#ifndef INDEX_INVOKE
#define INDEX_INVOKE(s, data, elem) \
            BOOST_PP_TUPLE_ELEM(BOOST_PP_TUPLE_SIZE(elem),1,elem)
#endif
#
///генерация списка индексов(через запятую)
#ifndef GENERATE_INDEX_LIST
#define GENERATE_INDEX_LIST(sequence)        \
            BOOST_PP_SEQ_ENUM(               \
                    BOOST_PP_SEQ_TRANSFORM(  \
                            INDEX_INVOKE,    \
                            BOOST_PP_EMPTY,  \
                            sequence)        \
                             )
#endif
#
///из списка индексов получаем mpl::vector_c
#ifndef TUPLE_INDEXES
#define TUPLE_INDEXES(sequence)     \
            boost::mpl::vector_c<int, GENERATE_INDEX_LIST(sequence)>
#endif
#
///делаем typedef
#ifndef INDEX_TYPEDEF_I
#define INDEX_TYPEDEF_I(sequence) \
            typedef typename TUPLE_INDEXES(sequence) indexes;
#endif
#
#ifndef INDEX_TYPEDEF
#define INDEX_TYPEDEF(sequence) INDEX_TYPEDEF_I(sequence)
#endif
#
#
///делаем дружественную функцию для доступа к полям
#ifndef FRIEND_RETURN_FIELD
#define FRIEND_RETURN_FIELD \
                template<std::size_t index,                         \
                         typename T>                                \
                friend decltype(auto) refl::return_field(T&& );
#endif
#
#
/*#define FRIEND_FOR_EACH_FIELDS_IMPL \
            template<typename T,    \
                     typename seq,  \
                     size_t I>      \
            friend struct refl::aux::for_each_fields_impl;
# */
#
/// дружественная функция для доступа к списку индексов
#ifndef FRIEND_FOR_EACH_FIELDS
#define FRIEND_FOR_EACH_FIELDS                                          \
            template<typename T,                                        \
                     typename F>                                        \
            friend decltype(auto) refl::for_each_fields(T&& , F&& );
#endif
#
///дружественная функция для доступа к списку индексов
#ifndef FRIEND_FOR_EACH_INDEX
#define FRIEND_FOR_EACH_INDEX                                           \
            template<typename T,                                        \
                     typename F>                                        \
            friend decltype(auto) refl::for_each_index(T&& , F&& );
#endif
#
#
///итоговый макрос для рефлексии полей
//#ifndef REFLECTABLE_FIELDS
#define REFLECTABLE_FIELDS(sequence)            \
            FRIEND_RETURN_FIELD                 \
            FRIEND_FOR_EACH_FIELDS              \
            FRIEND_FOR_EACH_INDEX               \
            REFLECTABLE_FIELDS_I(sequence)      \
            INDEX_TYPEDEF(sequence)
#
//#endif
#
#
#
#endif // FIELDS_REFLECTION_HPP
