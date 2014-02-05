#pragma once
#include <ios>                          // xalloc
#include <ostream>                      // iword

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
public:
        setnotation() noexcept = default;
        setnotation(int flag) noexcept: flag_{flag} {}

        template<class CharT, class Traits>
        friend auto&
        operator<<(std::basic_ostream<CharT, Traits>& ostr, setnotation const& n)
        {
                ostr.iword(manip::notation()) = n.flag_;
                return ostr;
        }

private:
        int flag_{};
};

}       // namespace dctl
