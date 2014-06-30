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
        boost::totally_ordered< Value<rules::International> >   // !=, >=, >, <=
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

        template<class Move>
        explicit Value(Move const& m)
        :
                num_pieces_{m.captured_pieces().size()}
        {
                assert(invariant());
        }

        // modifiers

        void increment(bool /* is_king */)
        {
                assert(!full());
                ++num_pieces_;
                assert(invariant());
        }

        void increment() { increment(true); }

        void decrement(bool /* is_king */)
        {
                assert(!empty());
                --num_pieces_;
                assert(invariant());
        }

        void decrement() { decrement(true); }

        // queries

        auto size() const
        {
                return num_pieces_;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend auto operator==(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ == rhs.num_pieces_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend auto operator<(Value const& lhs, Value const& rhs)
        {
                return lhs.num_pieces_ < rhs.num_pieces_;
        }

private:
        // contracts

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

        int num_pieces_{};
};

}       // namespace successor
}       // namespace dctl
