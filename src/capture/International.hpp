#pragma once
#include <limits>                       // numeric_limits
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/operators.hpp>          // totally_ordered
#include "Value_fwd.hpp"                // Value (primary template)

namespace dctl {

namespace rules { struct International; }

namespace capture {

// partial specialization for International draughts
template<typename Board>
struct Value<rules::International, Board>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::International, Board> >
{
public:
        // structors

        Value()
        :
                num_pieces_(0)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void increment()
        {
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement()
        {
                BOOST_ASSERT(!empty());
                --num_pieces_;
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
                return num_pieces_ < other.num_pieces_;
        }

        bool operator==(Value const& other) const
        {
                return num_pieces_ == other.num_pieces_;
        }

private:
        bool invariant() const
        {
                return 0 <= num_pieces_ && num_pieces_ < std::numeric_limits<int>::max();
        }

        bool empty() const
        {
                return num_pieces_ == 0;
        }

        // representation

        int num_pieces_;
};

}       // namespace capture
}       // namespace dctl
