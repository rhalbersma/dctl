#pragma once
#include "ValueInterface.hpp"
#include "../rules/Rules.h"
#include "../utility/IntegerTypes.hpp"

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
        Value()
        :
                num_pieces_(0),
                num_kings_(0)
        {
        }

        // predicates
        bool operator<(const Value<variant::Spanish>& other) const
        {
                // 15 b)
                if (num_pieces_ < other.num_pieces_)
                        return true;
                if (num_pieces_ > other.num_pieces_)
                        return false;

                // 15 c)
                return num_kings_ < other.num_kings_;
        }

        bool operator==(const Value<variant::Spanish>& other) const
        {
                return (
                        (num_pieces_ == other.num_pieces_) &&
                         (num_kings_ == other.num_kings_)
                );
        }

private:
        // implementation
        virtual bool do_is_large(BitBoard /* captured_pieces */ ) const
        {
                return num_pieces_ >= rules::large_capture<variant::Spanish>::value; 
        }

        virtual void do_increment(BitBoard target_sq, BitBoard king_targets)
        {
                ++num_pieces_;
                if (target_sq & king_targets)
                        ++num_kings_;
        }

        virtual void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets)
                        --num_kings_;
                --num_pieces_;
        }       

        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
};
        
}       // namespace capture
}       // namespace dctl
