#pragma once
#include <boost/operators.hpp>                  // totally_ordered
#include <cassert>                              // assert
#include <limits>                               // numeric_limits

namespace dctl {
namespace international {

class Value
:
        boost::totally_ordered< Value >   // !=, >=, >, <=
{
public:
        // constructors

        constexpr Value() = default;

        explicit constexpr Value(int n) noexcept
        :
                num_pieces_{n}
        {}

        template<class U>
        explicit constexpr Value(U const& u) noexcept
        :
                Value{u.num_pieces()}
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
        // representation

        int num_pieces_{};
};

}       // namespace international
}       // namespace dctl
