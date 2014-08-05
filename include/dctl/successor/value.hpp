#pragma once
#include <boost/operators.hpp>          // totally_ordered

namespace dctl {
namespace successor {

class Value
:
        boost::totally_ordered< Value >  // < >= > <= == !=
{
public:
        // predicates

        // operator!= provided by boost::totally_ordered
        friend constexpr auto
        operator==(Value const& /* lhs */, Value const& /* rhs */) noexcept
        {
                return true;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend constexpr auto
        operator<(Value const& /* lhs */, Value const& /* rhs */) noexcept
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
