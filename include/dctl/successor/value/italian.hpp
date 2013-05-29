#pragma once
#include <algorithm>                            // count
#include <iterator>                             // begin, end
#include <limits>                               // numeric_limits
#include <tuple>                                // tie
#include <utility>                              // make_pair
#include <vector>                               // vector
#include <boost/assert.hpp>                     // BOOST_ASSERT
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/italian_fwd.hpp>  // Italian

namespace dctl {
namespace successor {

// specialization for Italian draughts
template<>
class Value<rules::Italian>
:
        // Curiously Recurring Template Pattern (CRTP)
        private boost::totally_ordered< Value<rules::Italian> > // < >= > <= == !=
{
public:
        // structors

        Value()
        :
                num_pieces_(0),
                num_kings_(0),
                is_with_king_(false),
                king_order_()
        {
                BOOST_ASSERT(invariant());
        }

        explicit Value(std::tuple< int, int, bool, std::vector<int> > const& t)
        :
                Value(std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t))
        {}

        Value(int pawns, int kings, bool with, std::vector<int> order)
        :
                num_pieces_(pawns + kings),
                num_kings_(kings),
                is_with_king_(with),
                king_order_(order)
        {}

        // modifiers

        void increment(bool is_king)
        {
                BOOST_ASSERT(!full());
                if (is_king) {
                        king_order_.push_back(-num_pieces_);
                        ++num_kings_;
                }
                ++num_pieces_;
                BOOST_ASSERT(invariant());
        }

        void decrement(bool is_king)
        {
                BOOST_ASSERT(!empty());
                --num_pieces_;
                if (is_king) {
                        --num_kings_;
                        BOOST_ASSERT(king_order_.back() == -num_pieces_);
                        king_order_.pop_back();
                }
                BOOST_ASSERT(invariant());
        }

        void toggle_with_king()
        {
                is_with_king_ ^= true;
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

        bool is_with_king() const
        {
                return is_with_king_;
        }
/*
        // should also work with auto alone, but Clang 3.3 only accepts decltype(auto)
        decltype(auto) king_order() const
        {
                return std::make_pair(king_order_.cbegin(), king_order_.cend());
        }
*/
        // predicates

        // operator!= provided by boost::totally_ordered
        friend bool operator==(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator==
                // NOTE: this will -in turn- delegate to std::vector::operator== for the last tuple element
                return (
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_, lhs.king_order_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_, rhs.king_order_)
                );
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend bool operator<(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator<
                // NOTE: this will -in turn- delegate to std::vector::operator< for the last tuple element
                return (
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_, lhs.king_order_) <
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_, rhs.king_order_)
                );
        }

private:
        // predicates

        bool invariant() const
        {
                return (
                                  0 <= num_kings_ &&
                         num_kings_ <= num_pieces_ &&
                        num_pieces_ <= std::numeric_limits<int>::max() &&
                         num_kings_ == static_cast<int>(king_order_.size()) &&
                        (!num_kings_ || is_with_king_)
                );
        }

        bool empty() const
        {
                return (0 == num_kings_) && (num_kings_ == num_pieces_) && king_order_.empty();
        }

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max();
        }

        // representation

        int num_pieces_;
        int num_kings_;
        bool is_with_king_;
        std::vector<int> king_order_;
};

}       // namespace successor
}       // namespace dctl
