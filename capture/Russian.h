#pragma once
#include <boost/config.hpp>
#include "ValueInterface.h"

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
        Value();

private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
        virtual bool do_is_promotion() const;
        virtual void do_toggle_promotion();

        BOOST_STATIC_CONSTANT(auto, TOGGLE = true);

        // representation
        bool promotion_;
};
        
}       // namespace capture
}       // namespace dctl
