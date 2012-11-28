#pragma once

// http://www.boost.org/doc/libs/1_51_0/libs/parameter/doc/html/reference.html#boost-parameter-max-arity
// currently defaults to 8, but could become infinite with C++11 variadic templates
#define BOOST_PARAMETER_MAX_ARITY 16    
#include <boost/parameter.hpp>
#undef BOOST_PARAMETER_MAX_ARITY
