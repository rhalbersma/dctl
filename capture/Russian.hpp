#pragma once
#include "Value_fwd.hpp"
#include "../utility/TotalOrderInterface.hpp"
#include "../utility/TotalOrderTrivialImpl.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// explicit specialization for Russian draughts
template<>
struct Value<variant::Russian>
:
        // Curiously Recurring Template Pattern (CRTP)
        public TotalOrderInterface< Value<variant::Russian> >,
        private TotalOrderTrivialImpl< Value<variant::Russian> >
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
        friend struct TotalOrderInterface< Value<variant::Russian> >;

        // representation
        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
