#pragma once
#include <cassert>                              // assert
#include <limits>                               // numeric_limits
#include <tuple>                                // tie
#include <boost/operators.hpp>                  // totally_ordered
#include <dctl/successor/value_fwd.hpp>         // Value (primary template)
#include <dctl/rules/variants/frisian_fwd.hpp>  // Frisian

namespace dctl {
namespace successor {

// specialization for Frisian draughts
template<>
class Value<rules::Frisian>
:
        boost::totally_ordered< Value<rules::Frisian> > // < >= > <= == !=
{
public:
        // structors

        Value() = default;

        // modifiers

        void increment(bool is_king)
        {
                assert(!full());
                num_kings_ += is_king;
                ++num_pieces_;
                assert(invariant());
        }

        void decrement(bool is_king)
        {
                assert(!empty());
                --num_pieces_;
                num_kings_ -= is_king;
                assert(invariant());
        }

        void toggle_with_king()
        {
                is_with_king_ ^= true;
                assert(invariant());
        }

        // queries

        auto size() const
        {
                return num_pieces_;
        }

        // predicates

        // operator!= provided by boost::totally_ordered
        friend auto operator==(Value const& lhs, Value const& rhs)
        {
                // delegate to std::tuple::operator==
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_, lhs.is_with_king_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_, rhs.is_with_king_)
                ;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend auto operator<(Value const& lhs, Value const& rhs)
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

        bool invariant() const
        {
                return
                                  0 <= num_kings_ &&
                         num_kings_ <= num_pieces_ &&
                        num_pieces_ <= std::numeric_limits<int>::max()
                ;
        }

        bool empty() const
        {
                return
                                 0 == num_kings_ &&
                        num_kings_ == num_pieces_
                ;
        }

        bool full() const
        {
                return num_pieces_ == std::numeric_limits<int>::max();
        }

        // representation

        int num_pieces_{};
        int num_kings_{};
        bool is_with_king_{};
};

}       // namespace successor
}       // namespace dctl
