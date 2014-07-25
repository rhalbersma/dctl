#pragma once
#include <algorithm>                            // count
#include <cassert>                              // assert
#include <iostream>
#include <iterator>                             // begin, end
#include <limits>                               // numeric_limits
#include <tuple>                                // tie
#include <utility>                              // make_pair
#include <vector>                               // vector
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/italian_fwd.hpp>  // Italian

namespace dctl {
namespace successor {

// specialization for Italian draughts
template<>
class Value<rules::Italian>
:
        boost::totally_ordered< Value<rules::Italian> > // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        explicit Value(std::tuple< int, int, bool, std::vector<int> > const& t)
        :
                Value {std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t)}
        {}

        Value(int p, int k, bool w, std::vector<int> o)
        :
                num_pieces_(p + k),
                num_kings_(k),
                is_with_king_(w),
                king_order_(o)
        {}

        template<class U>
        explicit Value(U const& u)
        :
                num_pieces_{u.num_pieces()},
                num_kings_{u.num_kings()},
                is_with_king_{u.is_with_king()},
                king_order_{begin(u.ordered_kings()), end(u.ordered_kings())}
        {
                assert(invariant());
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend auto
        operator==(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator==
                // NOTE: this will -in turn- delegate to std::vector::operator== for the last tuple element
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_, lhs.king_order_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_, rhs.king_order_)
                ;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend auto
        operator<(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator<
                // NOTE: this will -in turn- delegate to std::vector::operator< for the last tuple element
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_, lhs.king_order_) <
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_, rhs.king_order_)
                ;
        }

private:
        // contracts

        bool invariant() const
        {
                return
                                  0 <= num_kings_ &&
                         num_kings_ <= num_pieces_ &&
                        num_pieces_ <= std::numeric_limits<int>::max() &&
                         num_kings_ == static_cast<int>(king_order_.size()) &&
                        !(num_kings_ && !is_with_king_)
                ;
        }

        // representation

        int num_pieces_{};
        int num_kings_{};
        bool is_with_king_{};
        std::vector<int> king_order_{};
};

}       // namespace successor
}       // namespace dctl
