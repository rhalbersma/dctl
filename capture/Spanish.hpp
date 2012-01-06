#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Spanish; }

namespace capture {

// forward declaration of the primary template
template<typename> struct Value;

// explicit specialization for Spanish draughts
template<>
struct Value<variant::Spanish>
{
public:
        // constructors
        Value()
        :
                num_pieces_(0),
                num_kings_(0)
        {
                BOOST_ASSERT(invariant());
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

        int count() const
        {
                return num_pieces_; 
        }

        // modifiers
        void do_increment(BitBoard target_sq, BitBoard king_targets)
        {
                ++num_pieces_;
                if (target_sq & king_targets)
                        ++num_kings_;
                BOOST_ASSERT(invariant());
        }

        void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets)
                        --num_kings_;
                --num_pieces_;
                BOOST_ASSERT(invariant());
        }       

private:
        // implementation
        bool invariant() const
        {
                return num_kings_ <= num_pieces_;
        }

        // representation
        PieceCount num_pieces_;
        PieceCount num_kings_;
};

template<typename Rules>
void increment(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void increment(Value<variant::Spanish>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_increment(target_sq, king_targets);
}
 
template<typename Rules>
void decrement(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void decrement(Value<variant::Spanish>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_decrement(target_sq, king_targets);
}

}       // namespace capture
}       // namespace dctl
