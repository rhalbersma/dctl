#pragma once

namespace dctl {
namespace notation {

struct numeric {};      // numeric
struct algebraic {};    // algebraic

template<typename> struct Format        { typedef numeric type; };

template<char M, char J>
struct Token
{
        static const char move = M;
        static const char jump = J;
};

template<typename> struct Separator     { typedef Token<'-', 'x'> type; };

}       // namespace notation
}       // namespace dctl
