#pragma once
#include <limits>                               // numeric_limits
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include "Value_fwd.hpp"                        // Value (primary template)
#include "../rules/International_fwd.hpp"       // International

namespace dctl {
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
                BOOST_ASSERT(!full());
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

        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ < rhs.num_pieces_;
        }

        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ == rhs.num_pieces_;
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

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max() - 1;
        }

        // representation

        int num_pieces_;
};

}       // namespace capture
}       // namespace dctl
