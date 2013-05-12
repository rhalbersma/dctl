#pragma once
#include <limits>                               // numeric_limits
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/international_fwd.hpp>     // International

namespace dctl {
namespace successor {

// partial specialization for International draughts
template<>
struct Value<rules::International>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::International> >   // < >= > <= == !=
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

        // operator!= provided by boost::totally_ordered
        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ == rhs.num_pieces_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ < rhs.num_pieces_;
        }

private:
        bool invariant() const
        {
                return 0 <= num_pieces_ && num_pieces_ <= std::numeric_limits<int>::max();
        }

        bool empty() const
        {
                return 0 == num_pieces_;
        }

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max();
        }

        // representation

        int num_pieces_;
};

}       // namespace successor
}       // namespace dctl
