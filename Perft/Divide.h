#pragma once
#include "../Utilities/IntegerTypes.h"
#include <string>

template<typename> class Position;

class Divide
{
public:
        template<typename, typename Board>
        static void root(Position<Board>&, size_t);

private:
        template<typename Board>
        static void announce(const Position<Board>&, size_t, size_t);

        static void print_move(const std::string&, size_t);
        static void summary(NodeCount);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Divide.hpp"
