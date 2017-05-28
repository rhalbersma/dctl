#pragma once
#include <dctl/core/action/traits/notation.hpp>      // Notation
#include <xstd/type_traits.hpp>                 // to_underlying_type
#include <ios>                                  // ios_base, iword, xalloc

namespace dctl {

class setnotation
{
public:
        static auto index()
        {
                static auto const i = std::ios_base::xalloc();
                return i;
        }
};

inline
auto& algebraic(std::ios_base& str)
{
        str.iword(setnotation::index()) = xstd::to_underlying_type(Notation::algebraic);
        return str;
}

inline
auto& numeric(std::ios_base& str)
{
        str.iword(setnotation::index()) = xstd::to_underlying_type(Notation::numeric);
        return str;
}

inline
auto gnotation(std::ios_base& str)
{
        return static_cast<Notation>(str.iword(setnotation::index()));
}

}       // namespace dctl
