#pragma once
#include <boost/operators.hpp>  // totally_ordered
#include <cassert>              // assert
#include <tuple>                // tie

namespace dctl {
namespace spanish {

class Value
:
        boost::totally_ordered<Value>   // < >= > <= == !=
{
public:
        // constructors

        constexpr Value() = default;

        constexpr Value(int np, int nk) noexcept
        :
                num_pieces_{np},
                num_kings_{nk}
        {
                assert(invariant());
        }

        template<class Move>
        explicit constexpr Value(Move const& m) noexcept
        :
                Value{m.num_pieces(), m.num_kings()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                // delegate to std::tuple::operator==
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_) ==
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                ;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend /* constexpr */ auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                // delegate to std::tuple::operator<
                return
                        std::tie(lhs.num_pieces_, lhs.num_kings_) <
                        std::tie(rhs.num_pieces_, rhs.num_kings_)
                ;
        }

private:
        // contracts

        constexpr bool invariant() const noexcept
        {
                return 0 <= num_kings_ && num_kings_ <= num_pieces_;
        }

        // representation

        int num_pieces_{};
        int num_kings_{};
};

}       // namespace spanish
}       // namespace dctl
