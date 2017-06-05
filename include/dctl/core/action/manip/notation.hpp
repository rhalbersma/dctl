#pragma once
#include <dctl/core/rules/traits.hpp>   // notation
#include <xstd/type_traits.hpp>         // to_underlying_type
#include <ios>                          // ios_base, iword, xalloc

namespace dctl {
namespace core {

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
        str.iword(setnotation::index()) = xstd::to_underlying_type(notation::algebraic);
        return str;
}

inline
auto& numeric(std::ios_base& str)
{
        str.iword(setnotation::index()) = xstd::to_underlying_type(notation::numeric);
        return str;
}

inline
auto getnotation(std::ios_base& str)
{
        return static_cast<notation>(str.iword(setnotation::index()));
}

}       // namespace core
}       // namespace dctl
