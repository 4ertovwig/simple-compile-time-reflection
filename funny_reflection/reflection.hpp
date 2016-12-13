/******************************************************************************
     * File: reflection.hpp
     * Description: общий подключаемый файл
     * Created: 10 oct 2016
     * Author: Ilya Korotkov
     * Email: lord2402@mail.ru
******************************************************************************/
#ifndef REFLECTION_HPP
#define REFLECTION_HPP
#
#   include <functional>
#
#   include <boost/mpl/vector_c.hpp>
#   include <boost/mpl/for_each.hpp>
#   include <boost/mpl/at.hpp>
#   include <boost/mpl/size.hpp>
#   include <boost/mpl/pop_back.hpp>
#   include <boost/mpl/back.hpp>
#
#
#   include <boost/preprocessor/seq/for_each.hpp>
#   include <boost/preprocessor/punctuation/comma.hpp>
#   include <boost/preprocessor/seq/enum.hpp>
#   include <boost/preprocessor/seq/transform.hpp>
#
#if __cplusplus >= 201402L
    static_assert(true ,"OK");
#           /*реализация функций*/
#   include <../funny_reflection/help_functions_reflection.hpp>
#           /*макросы*/
#   include <../funny_reflection/fields_reflection.hpp>
#   include <../funny_reflection/function_reflection.hpp>
#
#else
    static_assert(false ,"need minimum c++14");
#endif
#
#endif // REFLECTION_HPP
