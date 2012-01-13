#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "../bit/Bit.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Italian; }

namespace capture {

// forward declaration of the primary template
template<typename> struct Value;

// explicit specialization for Italian draughts
template<>
struct Value<variant::Italian>
{
public:
        // constructors
        Value()
        :
                piece_order_(0),
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
                BOOST_ASSERT(invariant());
        }

        // predicates
        bool operator<(const Value<variant::Italian>& other) const
        {
                // Art. 6.6
                if (num_pieces_ < other.num_pieces_)
                        return true;
                if (num_pieces_ > other.num_pieces_)
                        return false;

                // Art. 6.7
                if (with_king_ < other.with_king_)
                        return true;
                if (with_king_ > other.with_king_)
                        return false;

                // Art. 6.8
                if (num_kings_ < other.num_kings_)
                        return true;
                if (num_kings_ > other.num_kings_)
                        return false;

                // Art. 6.9
                return piece_order_ < other.piece_order_;
        }

        bool operator==(const Value<variant::Italian>& other) const
        {
                return (
                        (piece_order_ == other.piece_order_) &&
                         (num_pieces_ == other.num_pieces_) &&
                          (num_kings_ == other.num_kings_) &&
                          (with_king_ == other.with_king_)
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
                if (target_sq & king_targets) {
                        ++num_kings_;
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                }
                BOOST_ASSERT(invariant());
        }

        void do_decrement(BitBoard target_sq, BitBoard king_targets)
        {
                if (target_sq & king_targets) {
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                        --num_kings_;
                }
                --num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void do_toggle_with_king()
        {
                with_king_ ^= toggle;
        }

private:
        // implementation
        bool invariant() const
        {
                return (
                        (num_kings_ <= num_pieces_) &&
                        (num_kings_ == bit::count(piece_order_)) &&
                        (!num_kings_ || with_king_)
                );
        }

        BOOST_STATIC_CONSTANT(auto, toggle = true);

        // representation
        BitBoard piece_order_;
        int num_pieces_;
        int num_kings_;
        bool with_king_;
};
     
template<typename Rules>
void increment(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void increment(Value<variant::Italian>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_increment(target_sq, king_targets);
}
 
template<typename Rules>
void decrement(Value<Rules>&, BitBoard, BitBoard);

template<> inline
void decrement(Value<variant::Italian>& v, BitBoard target_sq, BitBoard king_targets)
{
        v.do_decrement(target_sq, king_targets);
}

template<typename Rules>
void toggle_with_king(Value<Rules>&);

template<> inline
void toggle_with_king(Value<variant::Italian>& v)
{
        v.do_toggle_with_king();
}

}       // namespace capture
}       // namespace dctl
