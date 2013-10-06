#pragma once
#include <cassert>                              // assert
#include <limits>                               // numeric_limits
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/international_fwd.hpp>     // International

namespace dctl {
namespace successor {

// partial specialization for International draughts
template<>
class Value<rules::International>
:
        boost::totally_ordered< Value<rules::International> >   // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        explicit Value(int n)
        :
                num_pieces_{n}
        {
                assert(invariant());
        }

        // modifiers

        void increment()
        {
                assert(!full());
                ++num_pieces_;
                assert(invariant());
        }

        void decrement()
        {
                assert(!empty());
                --num_pieces_;
                assert(invariant());
        }

        // queries

        int size() const
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

        int num_pieces_ = 0;
};

}       // namespace successor
}       // namespace dctl
