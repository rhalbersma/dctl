#pragma once
#include <limits>                       // numeric_limits
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)
#include "../bit/Bit.hpp"               // count, reverse_singlet

namespace dctl {

namespace rules { struct Italian; }

namespace capture {

// partial specialization for Italian draughts
template<typename Board>
struct Value<rules::Italian, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Italian, Board> >
{
private:
        // typedefs

        typedef typename Board::bit_type BitType;

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
                        piece_order_ ^= bit::reverse_singlet<BitType>(num_pieces_);
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
                        piece_order_ ^= bit::reverse_singlet<BitType>(num_pieces_);
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

        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                if (lhs.num_pieces_ < rhs.num_pieces_) return true;
                if (lhs.num_pieces_ > rhs.num_pieces_) return false;
                if (lhs.with_king_  < rhs.with_king_ ) return true;
                if (lhs.with_king_  > rhs.with_king_ ) return false;
                if (lhs.num_kings_  < rhs.num_kings_ ) return true;
                if (lhs.num_kings_  > rhs.num_kings_ ) return false;
                return lhs.piece_order_ < rhs.piece_order_;
        }

        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                return (
                        ( lhs.num_pieces_  == rhs.num_pieces_  ) &&
                        ( lhs.with_king_   == rhs.with_king_   ) &&
                        ( lhs.num_kings_   == rhs.num_kings_   ) &&
                        ( lhs.piece_order_ == rhs.piece_order_ )
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

        int num_pieces_;
        int num_kings_;
        bool with_king_;
        BitType piece_order_;
};

}       // namespace capture
}       // namespace dctl
