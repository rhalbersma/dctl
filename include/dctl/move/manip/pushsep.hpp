#pragma once
#include <ios>                          // xalloc
#include <ostream>                      // iword

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
public:
        setpushsep(char flag) noexcept : flag_{flag} {}

        template<class CharT, class Traits>
        friend auto&
        operator<<(std::basic_ostream<CharT, Traits>& ostr, setpushsep const& p)
        {
                ostr.iword(manip::pushsep()) = static_cast<int>(p.flag_);
                return ostr;
        }

private:
        char flag_;
};

}       // namespace dctl
