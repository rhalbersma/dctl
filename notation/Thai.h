#pragma once

namespace dctl {

namespace variant { struct Thai; }

namespace notation {

template<char, char> struct Token;
template<typename> struct Separator;
template<> struct Separator<variant::Thai>      { typedef Token<'-', '-'> type; };

}       // namespace notation
}       // namespace dctl
