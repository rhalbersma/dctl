#pragma once
#include <ios>          // xalloc
#include <ostream>      // iword

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
        char flag_;
public:
        explicit setjumpsep(char flag) noexcept : flag_{flag} {}

        template<class CharT, class Traits>
        friend auto&
        operator<<(std::basic_ostream<CharT, Traits>& ostr, setjumpsep const& m)
        {
                ostr.iword(manip::jumpsep()) = static_cast<int>(m.flag_);
                return ostr;
        }
};

}       // namespace dctl
