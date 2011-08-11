#pragma once
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace rules { struct Spanish; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<rules::Spanish>: public ValueInterface<rules::Spanish>
{
public:
        // constructors
        Value();

        // predicates
        bool operator< (const Value<rules::Spanish>&) const;
        bool operator==(const Value<rules::Spanish>&) const;

private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
        virtual void do_increment(BitBoard, BitBoard);
        virtual void do_decrement(BitBoard, BitBoard);        

        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
};
        
}       // namespace capture
}       // namespace dctl
