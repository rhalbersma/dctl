#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Types.hpp"

namespace dctl {
namespace notation {

template<typename>
struct Format
{
        typedef numeric type;
};

template<char M, char J>
struct Token
{
        BOOST_STATIC_CONSTANT(auto, move = M);
        BOOST_STATIC_CONSTANT(auto, jump = J);
};

template<typename>
struct Separator
{
        typedef Token<'-', 'x'> type;
};

}       // namespace notation
}       // namespace dctl
