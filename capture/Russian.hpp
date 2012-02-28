#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Value_fwd.hpp"

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// explicit specialization for Russian draughts
template<>
struct Value<variant::Russian>
{
public:
        // constructors
        Value()
        :
                promotion_(false)
        {
        }

        // predicates
        bool is_promotion() const
        {
                return promotion_;
        }

        // modifiers
        void toggle_promotion()
        {
                promotion_ ^= toggle;
        } 

private:
        // implementation
        BOOST_STATIC_CONSTANT(auto, toggle = true);

        // representation
        bool promotion_;
};

}       // namespace capture
}       // namespace dctl
