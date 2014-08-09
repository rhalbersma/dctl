#pragma once
#include <boost/operators.hpp>  // totally_ordered

namespace dctl {
namespace czech {

template<class Move>
class Value
:
        boost::totally_ordered<Value<Move>>     // < >= > <= == !=
{
public:
        // constructors

        constexpr Value() = default;

        explicit constexpr Value(bool b) noexcept
        :
                is_with_king_{b}
        {}

        explicit constexpr Value(Move const& m) noexcept
        :
                Value{m.is_with_king()}
        {}

        // predicates

        // operator!= provided by boost::totally_ordered
        friend constexpr auto
        operator==(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.is_with_king_ == rhs.is_with_king_;
        }

        // operator>=, operator>, operator<= provided by boost::totally_ordered
        friend constexpr auto
        operator<(Value const& lhs, Value const& rhs) noexcept
        {
                return lhs.is_with_king_ < rhs.is_with_king_;
        }

private:
        // representation

        bool is_with_king_{};
};

}       // namespace czech
}       // namespace dctl
