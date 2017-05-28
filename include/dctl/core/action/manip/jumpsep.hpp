#pragma once
#include <ios>  // ios_base, iword, xalloc

namespace dctl {

class setjumpsep
{
        char word;
public:
        explicit setjumpsep(char c) noexcept
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
auto& operator<<(std::ios_base& str, setjumpsep const& m)
{
        str.iword(m.index()) = m.iword();
        return str;
}

inline
auto jumpsep(std::ios_base& str)
{
        return static_cast<char>(str.iword(setjumpsep::index()));
}

}       // namespace dctl
