#pragma once
#include <ios>          // xalloc
#include <ostream>      // iword

namespace dctl {
namespace manip {

enum class PositionFormat { diag = 1, fen = 2 };

inline
auto position_format()
{
        static auto const slot = std::ios_base::xalloc();
        return slot;
}

}       // namespace manip

template<class CharT, class Traits>
auto& diag(std::basic_ostream<CharT, Traits>& ostr)
{
        ostr.iword(manip::position_format()) = static_cast<int>(manip::PositionFormat::diag);
        return ostr;
}

template<class CharT, class Traits>
auto& fen(std::basic_ostream<CharT, Traits>& ostr)
{
        ostr.iword(manip::position_format()) = static_cast<int>(manip::PositionFormat::fen);
        return ostr;
}

}       // namespace dctl
