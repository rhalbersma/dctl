#pragma once
#include <ios>  // ios_base, iword, xalloc

namespace dctl {
namespace manip {

inline
auto pushsep()
{
        static auto const slot = std::ios_base::xalloc();
        return slot;
}

}       // namespace manip

class setpushsep
{
        char flag;
public:
        explicit setpushsep(char f) noexcept
        :
                flag{f}
        {}

        friend auto& operator<<(std::ios_base& str, setpushsep const& m)
        {
                str.iword(manip::pushsep()) = m.flag;
                return str;
        }
};

}       // namespace dctl
