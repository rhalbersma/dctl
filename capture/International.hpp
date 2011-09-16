#pragma once
#include "ValueInterface.hpp"
#include "../rules/Rules.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct International; }

namespace capture {

// forward declaration of the primary template
template<typename> class Value;

// explicit specialization for International draughts
template<>
class Value<variant::International>
: 
        public ValueInterface
{
public:
        // constructors
        Value()
        :
                num_pieces_(0)
        {
        }

        // predicates
        bool operator<(const Value<variant::International>& other) const
        {
                return num_pieces_ < other.num_pieces_;
        }

        bool operator==(const Value<variant::International>& other) const
        {
                return num_pieces_ == other.num_pieces_;
        }

private:
        // implementation
        virtual bool do_is_large(BitBoard /* captured_pieces */ ) const
        {
                return num_pieces_ >= rules::large_capture<variant::International>::value; 
        }

        virtual void do_increment(BitBoard, BitBoard)
        {
                ++num_pieces_;
        }

        virtual void do_decrement(BitBoard, BitBoard)
        {
                --num_pieces_;
        }      

        // representation
        PieceCount num_pieces_;
};
        
}       // namespace capture
}       // namespace dctl
