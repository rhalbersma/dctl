#pragma once
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace variant { struct Spanish; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for Spanish draughts
template<>
class Value<variant::Spanish>
: 
        public ValueInterface
{
public:
        // constructors
        Value();

        // predicates
        bool operator< (const Value<variant::Spanish>&) const;
        bool operator==(const Value<variant::Spanish>&) const;

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
