#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

namespace dctl {
namespace capture {

// primary template
template<typename Rules, typename Board>
struct Value
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<Rules, Board> >
{
        // predicates

        friend bool operator<(Value const & /* lhs */, Value const& /* rhs */)
        {
                return false;
        }

        friend bool operator==(Value const & /* lhs */, Value const& /* rhs */)
        {
                return true;
        }
};

}       // namespace capture
}       // namespace dctl
