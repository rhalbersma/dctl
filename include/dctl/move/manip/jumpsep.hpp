#pragma once
#include <ios>  // ios_base, iword, xalloc

namespace dctl {
namespace manip {

inline
auto jumpsep()
{
        static auto const slot = std::ios_base::xalloc();
        return slot;
}

}       // namespace manip

class setjumpsep
{
        char flag;
public:
        explicit setjumpsep(char f) noexcept
        :
                flag{f}
        {}

        friend auto& operator<<(std::ios_base& str, setjumpsep const& m)
        {
                str.iword(manip::jumpsep()) = m.flag;
                return str;
        }
};

}       // namespace dctl
