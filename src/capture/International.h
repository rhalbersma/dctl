#pragma once
#include "ValueInterface.h"
#include "../utils/IntegerTypes.h"

namespace dctl {

namespace variant { struct International; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

template<>
class Value<variant::International>
: 
        public ValueInterface
{
public:
        // constructors
        Value();

        // predicates
        bool operator< (const Value<variant::International>&) const;
        bool operator==(const Value<variant::International>&) const;

private:
        // implementation
        virtual bool do_is_large(BitBoard) const;
        virtual void do_increment(BitBoard, BitBoard);
        virtual void do_decrement(BitBoard, BitBoard);        

        // representation
        PieceCount num_pieces_;
};
        
}       // namespace capture
}       // namespace dctl
