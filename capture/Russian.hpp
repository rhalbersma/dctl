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
        // structors

        Value()
        :
                promotion_(false)
        {
        }

        // modifiers

        void toggle_promotion()
        {
                promotion_ ^= true;
        }

        // queries

        bool is_promotion() const
        {
                return promotion_;
        }

        // predicates

        bool operator<(const Value& /* other */) const
        {
                return false;
        }

        bool operator==(const Value& /* other */) const
        {
                return true;
        }

private:
        // representation

        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
