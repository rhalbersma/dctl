#pragma once
#include <limits>                       // numeric_limits
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)
#include "../bit/Bit.hpp"               // count, reverse_singlet
#include "../utility/IntegerTypes.hpp"

namespace dctl {

namespace rules { struct Italian; }

namespace capture {

// explicit specialization for Italian draughts
template<>
struct Value<rules::Italian>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Italian> >
{
public:
        // structors

        Value()
        :
                piece_order_(0),
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
                BOOST_ASSERT(invariant());
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
                BOOST_ASSERT(!empty(is_captured_king));
                --num_pieces_;
                if (is_captured_king) {
                        --num_kings_;
                        piece_order_ ^= bit::reverse_singlet<BitBoard>(num_pieces_);
                }
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {
                with_king_ ^= true;
                BOOST_ASSERT(invariant());
        }

        // queries

        int count() const
        {
                return num_pieces_;
        }

        // predicates

        bool operator<(Value const& other) const
        {
                return (
                         ( num_pieces_  < other.num_pieces_  ) || (( num_pieces_ == other.num_pieces_ ) &&
                        (( with_king_   < other.with_king_   ) || (( with_king_  == other.with_king_  ) &&
                        (( num_kings_   < other.num_kings_   ) || (( num_kings_  == other.num_kings_  ) &&
                        (( piece_order_ < other.piece_order_ )))))))
                );
        }

        bool operator==(Value const& other) const
        {
                return (
                        ( num_pieces_  == other.num_pieces_  ) &&
                        ( with_king_   == other.with_king_   ) &&
                        ( num_kings_   == other.num_kings_   ) &&
                        ( piece_order_ == other.piece_order_ )
                );
        }

private:
        bool invariant() const
        {
                return (
                        (0 <= num_kings_) && 
                        (num_kings_ <= num_pieces_) &&
                        (num_pieces_ < std::numeric_limits<int>::max()) &&
                        (num_kings_ == bit::count(piece_order_)) &&
                        (!num_kings_ || with_king_)
                );
        }

        bool empty(bool is_captured_king) const
        {
                return is_captured_king? (num_kings_ == 0 && piece_order_ == 0) : (num_pieces_ == 0); 
        }

        // representation

        BitBoard piece_order_;
        int num_pieces_;
        int num_kings_;
        bool with_king_;
};

}       // namespace capture
}       // namespace dctl
