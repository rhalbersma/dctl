#pragma once
#include <sstream>                      // stringstream
#include <stdexcept>                    // invalid_argument
#include <string>                       // string

namespace dctl {

struct Bound
{
        enum {
                lower = 1,
                upper = 2,
                exact = lower ^ upper
        };

        static int type(int value, int alpha, int beta)
        {
                return
                        value >= beta  ? lower :
                        value <= alpha ? upper :
                                         exact
                ;
        }

        static std::string print(int bound)
        {
                std::stringstream sstr;
                switch (bound) {
                case lower : sstr << ">="; break;
                case upper : sstr << "<="; break;
                case exact : sstr << "=="; break;
                default    : throw std::invalid_argument("Search bound must be >=, <= or ==");
                }
                return sstr.str();
        }
};

}       // namespace dctl
