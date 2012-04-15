#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"

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
        bool operator<(const Value& /* other */) const
        {
                return false;
        }

        bool operator==(const Value& /* other */) const
        {
                return true;
        }
};

template<typename Rules>
bool is_totally_ordered(const Value<Rules>& left, const Value<Rules>& right)
{
        const auto equivalent = !((left < right) || (right < left));
        const auto equal = left == right;
        return equivalent == equal;
}

}       // namespace capture
}       // namespace dctl
