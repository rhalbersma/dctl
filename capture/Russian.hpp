#pragma once
#include <boost/config.hpp>
#include "ValueInterface.hpp"
#include "../bit/Bit.h"
#include "../rules/Rules.h"

namespace dctl {

namespace variant { struct Russian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for Russian draughts
template<>
class Value<variant::Russian>
: 
        public ValueInterface
{
public:
        // constructors
        Value()
        :
                promotion_(false)
        {
        }

private:
        // implementation
        virtual bool do_is_large(BitBoard captured_pieces) const
        {
                return bit::count(captured_pieces) >= rules::large_capture<variant::Russian>::value;
        }

        virtual bool do_is_promotion() const
        {
                return promotion_;
        }

        virtual void do_toggle_promotion()
        {
                promotion_ ^= TOGGLE;
        } 

        BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        bool promotion_;
};
        
}       // namespace capture
}       // namespace dctl
