#pragma once
#include <ios>                          // xalloc
#include <ostream>                      // iword

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
public:
        explicit setjumpsep(char flag) noexcept : flag_{flag} {}

        template<class CharT, class Traits>
        friend auto&
        operator<<(std::basic_ostream<CharT, Traits>& ostr, setjumpsep const& j)
        {
                ostr.iword(manip::jumpsep()) = static_cast<int>(j.flag_);
                return ostr;
        }

private:
        char flag_;
};

}       // namespace dctl
