#pragma once
#include <ios>  // ios_base, iword, xalloc

namespace dctl {
namespace manip {

inline
auto notation()
{
        static auto const slot = std::ios_base::xalloc();
        return slot;
}

}       // namespace manip

class setnotation
{
        int flag{};
public:
        explicit setnotation(int f) noexcept
        :
                flag{f}
        {}

        friend auto& operator<<(std::ios_base& str, setnotation const& m)
        {
                str.iword(manip::notation()) = m.flag;
                return str;
        }
};

}       // namespace dctl
