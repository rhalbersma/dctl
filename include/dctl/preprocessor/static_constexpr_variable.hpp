#pragma once

/*

        Static constexpr variables that can ODR-safely be defined in headers.

        The macro provides a reference inside an unnamed namespace
        to an object instance in an implementation class template.

        Each object in an unnamed namespace inside a header generates
        a unique instance in every translation unit that includes its header.
        However, to prevent ODR violations, it is important that the objects
        in e.g. multiple instantiations of a function template are the same.

        For references it doesn't matter that they have a different identity;
        as long as they refer to the same object instance in an implementation
        class template.

        See the following discussion on the Boost mailinglist for more details:
        http://lists.boost.org/Archives/boost/2007/06/123380.php

*/

#define DCTL_PP_STATIC_CONSTEXPR_VARIABLE(type, var, value)                                     \
        namespace var##detail {                                                                 \
                template<class>                                                                 \
                struct wrapper                                                                  \
                {                                                                               \
                        static constexpr type var = value;                                      \
                };                                                                              \
                template<class T> constexpr type wrapper<T>::var;                               \
                struct placeholder {};                                                          \
        }                                                                                       \
        namespace {                                                                             \
                auto const& var = var##detail::wrapper<var##detail::placeholder>::var;          \
        }
