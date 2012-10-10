#pragma once

// Trick by Andrei Alexandrescu, Dr. Dobb's, October 01, 2001
// http://www.drdobbs.com/cpp/184403750?_requestid=260625
//
// Int2Type gives a distinct type for each constant integral value passed.
// This is because different template instantiations are distinct types,
// so Int2Type<0> is different from Int2Type<1> and so on.
//
// This "tag dispatching" simulates partial specialization of function templates.

namespace dctl {

template<int V>
struct Int2Type
{
        enum { value = V };
};

namespace utils {

// Private default constructor ensures classes derived from class noncopyable cannot be copied.
// Inspired by the boost::noncopyable class which privately declares -but does not define- 
// the copy constructor and assignment operator
class nonconstructible 
{
private:
        nonconstructible();
};

}       // namespace utils
}       // namespace dctl
