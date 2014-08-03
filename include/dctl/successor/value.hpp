#pragma once
#include <dctl/successor/value_fwd.hpp> // Value (primary template)
#include <boost/operators.hpp>          // totally_ordered

namespace dctl {
namespace successor {

// primary template
template<class Rules>
class Value
:
        boost::totally_ordered< Value<Rules> >  // < >= > <= == !=
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
