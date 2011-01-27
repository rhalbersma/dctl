#pragma once
#include "../Utilities/IntegerTypes.h"
#include <string>

template<typename> class Position;

namespace TreeWalk {

class Divide
{
public:
        template<typename, typename B> static NodeCount divide(const Position<B>&, size_t);

private:
        template<typename B> static void announce(const Position<B>&, size_t, size_t);

        static void print_move(const std::string&, size_t);
        static void summary(NodeCount);
};

}       // namespace TreeWalk

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Divide.hpp"
