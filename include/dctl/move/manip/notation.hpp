#pragma once
#include <ios>          // xalloc
#include <ostream>      // iword

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
        int flag_{};
public:
        setnotation() = default;
        explicit setnotation(int flag) noexcept : flag_{flag} {}

        template<class CharT, class Traits>
        friend auto&
        operator<<(std::basic_ostream<CharT, Traits>& ostr, setnotation const& m)
        {
                ostr.iword(manip::notation()) = m.flag_;
                return ostr;
        }
};

}       // namespace dctl
