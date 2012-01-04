#pragma once
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/assert.hpp>             // BOOST_ASSERT

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
                return (
                        value >= beta  ? lower : 
                        value <= alpha ? upper : 
                                         exact
                );
        }

        static std::string print(int bound)
        {
                std::stringstream sstr;
                switch(bound) {
                case lower:
                        sstr << ">=";
                        break;
                case upper:
                        sstr << "<=";
                        break;
                case exact:
                        sstr << "==";
                        break;
                default:
                        BOOST_ASSERT(false);
                        break;
                }
                return sstr.str();
        }
};

}       // namespace dctl
