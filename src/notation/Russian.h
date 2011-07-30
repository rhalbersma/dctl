#pragma once

namespace dctl {

namespace rules { struct Russian; }

namespace notation {

struct algebraic;
template<typename> struct Format;
template<> struct Format<rules::Russian>        { typedef algebraic type; };

template<char, char> struct Token;
template<typename> struct Separator;
template<> struct Separator<rules::Russian>     { typedef Token<'-', ':'> type; };

}       // namespace notation
}       // namespace dctl
