#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

namespace dctl {
namespace capture {

// primary template
template<typename Rules>
struct Value
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<Rules> >
{
        // predicates

        bool operator<(Value const& /* other */) const
        {
                return false;
        }

        bool operator==(Value const& /* other */) const
        {
                return true;
        }
};

}       // namespace capture
}       // namespace dctl
