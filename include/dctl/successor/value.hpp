#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include <dctl/successor/value_fwd.hpp> // Value (primary template)

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
        friend bool operator==(Value const& /* lhs */, Value const& /* rhs */)
        {
                return true;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const& /* lhs */, Value const& /* rhs */)
        {
                return false;
        }
};

}       // namespace successor
}       // namespace dctl
