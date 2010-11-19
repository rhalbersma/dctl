#pragma once
#include "IntegerTypes.h"

template<typename T>
struct DeBruijn
{
        static const T SEQUENCE;
        static const size_t SHIFT;
        static const size_t TABLE[];
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "DeBruijn.hpp"
