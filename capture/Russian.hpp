#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// explicit specialization for Russian draughts
template<>
struct Value<variant::Russian>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<variant::Russian> >
{
public:
        // constructors
        Value()
        :
                promotion_(false)
        {
        }

        // predicates
        bool operator<(const Value<variant::Russian>& /* other */) const
        {
                return false;
        }

        bool operator==(const Value<variant::Russian>& /* other */) const
        {
                return true;
        }

        bool is_promotion() const
        {
                return promotion_;
        }

        // modifiers
        void toggle_promotion()
        {
                promotion_ ^= true;
        }

private:
        // representation
        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
