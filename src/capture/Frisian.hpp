#pragma once
#include <limits>                       // numeric_limits
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)
#include "../rules/Frisian_fwd.hpp"     // Frisian

namespace dctl {
namespace capture {

// partial specialization for Frisian draughts
template<typename Board>
struct Value<rules::Frisian, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Frisian, Board> >
{
public:
        // structors

        Value()
        :
                num_pieces_(0),
                num_kings_(0),
                with_king_(false)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void increment(bool is_captured_king)
        {
                BOOST_ASSERT(!full());
                num_kings_ += is_captured_king;
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_captured_king)
        {
                BOOST_ASSERT(!empty(is_captured_king));
                --num_pieces_;
                num_kings_ -= is_captured_king;
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
                auto const delta_kings = lhs.num_kings_ - rhs.num_kings_;
                auto const delta_pawns = lhs.num_pieces_ - rhs.num_pieces_ - delta_kings;

                // Art. 11
                if (delta_kings * delta_pawns < 0) {
                        // delta_kings and delta_pawns are both non-zero and have opposite sign
                        // [2 n - 1] pawns < [n] kings < [2 n] pawns
                        return delta_pawns + 2 * delta_kings - (delta_kings > 0) <  0;
                } else {
                        // delta_kings or delta_pawns is zero or they have equal sign
                        if (lhs.num_pieces_ < rhs.num_pieces_) return true;
                        if (lhs.num_pieces_ > rhs.num_pieces_) return false;
                        return lhs.with_king_ < rhs.with_king_;
                }
        }

        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                return (
                        ( lhs.num_pieces_ == rhs.num_pieces_ ) &&
                        ( lhs.num_kings_  == rhs.num_kings_  ) &&
                        ( lhs.with_king_  == rhs.with_king_  )
                );
        }

private:
        bool invariant() const
        {
                return (
                        (0 <= num_kings_) && 
                        (num_kings_ <= num_pieces_) && 
                        (num_pieces_ < std::numeric_limits<int>::max())
                );
        }

        bool empty(bool is_captured_king) const
        {
                return (is_captured_king? num_kings_ : num_pieces_) == 0; 
        }

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max() - 1;
        }

        // representation

        int num_pieces_;
        int num_kings_;
        bool with_king_;
};

}       // namespace capture
}       // namespace dctl