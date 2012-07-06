#pragma once
#include <limits>                       // numeric_limits
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

namespace dctl {

namespace rules { struct Spanish; }

namespace capture {

// partial specialization for Spanish draughts
template<typename Board>
struct Value<rules::Spanish, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Spanish, Board> >
{
public:
        // structors

        Value()
        :
                num_pieces_(0),
                num_kings_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void increment(bool is_captured_king)
        {
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

        // queries

        int count() const
        {
                return num_pieces_;
        }

        // predicates

        bool operator<(Value const& other) const
        {
                if (num_pieces_ < other.num_pieces_) return true;
                if (num_pieces_ > other.num_pieces_) return false;
                return num_kings_ < other.num_kings_;
        }

        bool operator==(Value const& other) const
        {
                return (
                        ( num_pieces_ == other.num_pieces_ ) &&
                        ( num_kings_  == other.num_kings_  )
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

        // representation

        int num_pieces_;
        int num_kings_;
};

}       // namespace capture
}       // namespace dctl
