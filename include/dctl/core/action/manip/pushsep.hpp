#pragma once
#include <ios>  // ios_base, iword, xalloc

namespace dctl {
namespace core {

class setpushsep
{
        char word;
public:
        explicit setpushsep(char c) noexcept
        :
                word{c}
        {}

        static auto index()
        {
                static auto const i = std::ios_base::xalloc();
                return i;
        }

        auto iword() const noexcept
        {
                return word;
        }
};

inline
auto& operator<<(std::ios_base& str, setpushsep const& m)
{
        str.iword(m.index()) = m.iword();
        return str;
}

inline
auto pushsep(std::ios_base& str)
{
        return static_cast<char>(str.iword(setpushsep::index()));
}

}       // namespace core
}       // namespace dctl
