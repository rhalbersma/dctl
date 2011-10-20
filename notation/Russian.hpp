#pragma once

namespace dctl {

namespace variant { struct Russian; }

namespace notation {

struct algebraic;
template<typename> struct Format;
//template<> struct Format<variant::Russian>      { typedef algebraic type; };

template<char, char> struct Token;
template<typename> struct Separator;
template<> struct Separator<variant::Russian>   { typedef Token<'-', ':'> type; };

}       // namespace notation
}       // namespace dctl
