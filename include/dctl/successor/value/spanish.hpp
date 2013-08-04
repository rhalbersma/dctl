#pragma once
#include <limits>                               // numeric_limits
#include <tuple>                                // get, tie
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/spanish_fwd.hpp>  // Spanish

namespace dctl {
namespace successor {

// specialization for Spanish draughts
template<>
class Value<rules::Spanish>
:
        boost::totally_ordered< Value<rules::Spanish> > // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        explicit Value(std::tuple<int, int> const& t)
        :
                Value{std::get<0>(t), std::get<1>(t)}
        {}

        Value(int pawns, int kings)
        :
                num_pieces_{pawns + kings},
                num_kings_{kings}
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

        int num_pawns() const
        {
                return num_pieces_ - num_kings_;
        }

        int num_kings() const
        {
                return num_kings_;
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

        int num_pieces_ = 0;
        int num_kings_ = 0;
};

}       // namespace successor
}       // namespace dctl
