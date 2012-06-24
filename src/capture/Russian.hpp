#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

namespace dctl {

namespace rules { struct Russian; }

namespace capture {

// partial specialization for Russian draughts
template<typename Board>
struct Value<rules::Russian, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Russian, Board> >
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

        bool operator<(Value const& /* other */) const
        {
                return false;
        }

        bool operator==(Value const& /* other */) const
        {
                return true;
        }

private:
        // representation

        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
