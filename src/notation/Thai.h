#pragma once

namespace dctl {

namespace rules { struct Thai; }

namespace notation {

template<char, char> struct Token;
template<typename> struct Separator;
template<> struct Separator<rules::Thai>        { typedef Token<'-', '-'> type; };

}       // namespace notation
}       // namespace dctl
