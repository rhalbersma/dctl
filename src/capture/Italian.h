#pragma once
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace variant { struct Italian; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for Italian draughts
template<>
class Value<variant::Italian>
: 
        public ValueInterface
{
public:
        // constructors
        Value();

        // predicates
        bool operator< (const Value<variant::Italian>&) const;
        bool operator==(const Value<variant::Italian>&) const;

private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
        virtual void do_increment(BitBoard, BitBoard);
        virtual void do_decrement(BitBoard, BitBoard);        
        virtual void do_toggle_with_king();

        static const bool TOGGLE = true;

        // representation
        BitBoard piece_order_;
        PieceCount num_pieces_;
        PieceCount num_kings_;
        bool with_king_;
};
        
}       // namespace capture
}       // namespace dctl
