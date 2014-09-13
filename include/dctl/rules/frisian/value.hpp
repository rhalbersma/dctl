#pragma once
#include <boost/operators.hpp>  // totally_ordered
#include <cassert>              // assert
#include <tuple>                // tie

namespace dctl {
namespace frisian {

class Value
:
        boost::totally_ordered<Value>   // < >= > <= == !=
{
public:
        // constructors

        constexpr Value() = default;

        constexpr Value(int np, int nk, bool wk) noexcept
        :
                num_pieces_{np},
                num_kings_{nk},
                is_with_king_{wk}
        {
                assert(invariant());
        }

        template<class Move>
        explicit constexpr Value(Move const& m) noexcept
        :
                Value{m.num_pieces(), m.num_kings(), m.is_with_king()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                // delegate to std::tuple::operator==
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_)
                ;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                auto const delta_kings  = lhs.num_kings_  - rhs.num_kings_ ;
                auto const delta_pieces = lhs.num_pieces_ - rhs.num_pieces_;
                auto const delta_pawns = delta_pieces - delta_kings;

                // Art. 11
                if (delta_kings * delta_pawns < 0)
                        // delta_kings and delta_pawns are both non-zero and have opposite sign
                        // [2 n - 1] pawns < [n] kings < [2 n] pawns
                        return delta_pawns + 2 * delta_kings - (delta_kings > 0) <  0;
                else
                        // delta_kings or delta_pawns is zero or they have equal sign
                        // delegate to std::tuple::operator<
                        return
                                std::tie(lhs.num_pieces_, lhs.is_with_king_) <
                                std::tie(rhs.num_pieces_, rhs.is_with_king_)
                        ;
        }

private:
        // contracts

        constexpr bool invariant() const
        {
                return 0 <= num_kings_ && num_kings_ <= num_pieces_;
        }

        // representation

        int num_pieces_{};
        int num_kings_{};
        bool is_with_king_{};
};

}       // namespace frisian
}       // namespace dctl
