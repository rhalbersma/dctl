#pragma once
#include <boost/config.hpp>

/*

Trick by Andrei Alexandrescu, Dr. Dobb's, October 01, 2001
http://www.drdobbs.com/cpp/184403750?_requestid=260625

Int2Type gives a distinct type for each constant integral value passed.
This is because different template instantiations are distinct types,
so Int2Type<0> is different from Int2Type<1> and so on.

This "tag dispatching" simulates partial specialization of function templates.

*/

namespace dctl {

template<int V>
struct Int2Type
{
        BOOST_STATIC_CONSTANT(auto, value = V);
};

// with C++0x variadic templates, the tuple clases below can be generalized
struct empty_tuple 
{
};

template<bool I> 
struct bool_tuple 
{
        BOOST_STATIC_CONSTANT(auto, first = I);
};

template<int I> 
struct int_tuple 
{
        BOOST_STATIC_CONSTANT(auto, first = I);
};

template<bool B, int I> 
struct bool_int_tuple 
{
        BOOST_STATIC_CONSTANT(auto, first = B);
        BOOST_STATIC_CONSTANT(auto, second = I);
};

namespace utility {

/*

The class nonconstructible privately declares -but does not define- the 
default constructor (which prevents derived classes from being copied).

The class nonconstructible was inspired by the boost::noncopyable class 
which privately declares -but does not define- the copy constructor and 
assignment operator (which prevents derived classes from being copied).

*/

class nonconstructible 
{
private:
        nonconstructible();
};

}       // namespace utility
}       // namespace dctl
