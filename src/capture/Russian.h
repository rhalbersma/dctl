#pragma once
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct Russian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::Russian>: public ValueInterface<rules::Russian>
{
public:
        // constructors
        Value();

private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
        virtual bool do_is_promotion() const;
        virtual void do_toggle_promotion();

        static const bool TOGGLE = true;

        // representation
        bool promotion_;
};
        
}       // namespace capture
}       // namespace dctl
