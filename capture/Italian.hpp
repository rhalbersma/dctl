#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Value_fwd.hpp"
#include "../bit/Bit.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace variant { struct Italian; }

namespace capture {

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
        void increment(bool is_captured_king)
        {
                if (is_captured_king) {
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                        ++num_kings_;
                }
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_captured_king)
        {
                --num_pieces_;
                if (is_captured_king) {
                        --num_kings_;
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                }
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {
                with_king_ ^= toggle;
                BOOST_ASSERT(invariant());
        }

private:
        // implementation
        bool invariant() const
        {
                return (
                        (0 <= num_kings_) &&
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
     
}       // namespace capture
}       // namespace dctl
