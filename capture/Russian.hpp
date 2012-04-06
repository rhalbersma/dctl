#pragma once
#include "Value_fwd.hpp"
#include "OrderInterface.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// explicit specialization for Russian draughts
template<>
struct Value<variant::Russian>
:
        public OrderInterface< Value<variant::Russian> >
{
public:
        // constructors
        Value()
        :
                promotion_(false)
        {
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
        friend struct OrderInterface< Value<variant::Russian> >;

        // predicates
        bool less(const Value<variant::Russian>& /* other */) const
        {
                return false;
        }

        bool equal(const Value<variant::Russian>& /* other */) const
        {
                return true;
        }

        // representation
        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
