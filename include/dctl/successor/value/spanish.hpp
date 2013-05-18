#pragma once
#include <limits>                               // numeric_limits
#include <tuple>                                // tie
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/spanish_fwd.hpp>  // Spanish

namespace dctl {
namespace successor {

// specialization for Spanish draughts
template<>
struct Value<rules::Spanish>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Spanish> > // < >= > <= == !=
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

        explicit Value(std::tuple<int, int> const& t)
        :
                Value(std::get<0>(t), std::get<1>(t))
        {}

        Value(int num_pawns, int num_kings)
        :
                num_pieces_(num_pawns + num_kings),
                num_kings_(num_kings)
        {
                BOOST_ASSERT(invariant());
        }

        // modifiers

        void increment(bool is_king)
        {
                BOOST_ASSERT(!full());
                num_kings_ += is_king;
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_king)
        {
                BOOST_ASSERT(!empty());
                --num_pieces_;
                num_kings_ -= is_king;
                BOOST_ASSERT(invariant());
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
                // delegate to std::tuple::operator==
                return (
                        std::tie(lhs.num_pieces_, lhs.num_kings_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                );
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator<
                return (
                        std::tie(lhs.num_pieces_, lhs.num_kings_) <
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                );
        }

private:
        // predicates

        bool invariant() const
        {
                return (
                                  0 <= num_kings_ &&
                         num_kings_ <= num_pieces_ &&
                        num_pieces_ <= std::numeric_limits<int>::max()
                );
        }

        bool empty() const
        {
                return 0 == num_kings_ && num_kings_ == num_pieces_;
        }

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max();
        }

        // representation

        int num_pieces_;
        int num_kings_;
};

}       // namespace successor
}       // namespace dctl
