#pragma once
#include <boost/operators.hpp>          // totally_ordered
#include <cassert>                      // assert
#include <tuple>                        // tie
#include <vector>                       // vector

namespace dctl {
namespace italian {

template<class Move>
class Value
:
        boost::totally_ordered<Value<Move>> // < >= > <= == !=
{
public:
        // constructors

        Value() = default;

        Value(int np, int nk, bool wk, std::vector<int> ko)
        :
                num_pieces_{np},
                num_kings_{nk},
                is_with_king_{wk},
                king_order_{ko}
        {
                assert(invariant());
        }

        explicit Value(Move const& m)
        :
                Value{m.num_pieces(), m.num_kings(), m.is_with_king(), std::vector<int>(begin(m.ordered_kings()), end(m.ordered_kings()))}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend auto
        operator==(Value const& lhs, Value const& rhs) noexcept
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
        operator<(Value const& lhs, Value const& rhs) noexcept
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
                        0 <= num_kings_ && num_kings_ <= num_pieces_ &&
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

}       // namespace italian
}       // namespace dctl
