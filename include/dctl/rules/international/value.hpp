#pragma once
#include <boost/operators.hpp>  // totally_ordered
#include <cassert>              // assert

namespace dctl {
namespace international {

class Value
:
        boost::totally_ordered<Value>   // !=, >=, >, <=
{
public:
        // constructors

        constexpr Value() = default;

        explicit constexpr Value(int n) noexcept
        :
                num_pieces_{n}
        {
                assert(invariant());
        }

        template<class Move>
        explicit constexpr Value(Move const& m) noexcept
        :
                Value{m.num_pieces()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend constexpr auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.num_pieces_ == rhs.num_pieces_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend constexpr auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.num_pieces_ < rhs.num_pieces_;
        }

private:
        // contracts

        constexpr bool invariant() const noexcept
        {
                return 0 <= num_pieces_;
        }

        // representation

        int num_pieces_{};
};

}       // namespace international
}       // namespace dctl
