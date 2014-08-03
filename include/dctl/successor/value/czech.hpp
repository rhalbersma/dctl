#pragma once
#include <dctl/successor/value_fwd.hpp> // Value (primary template)
#include <dctl/rules/czech_fwd.hpp>     // Czech
#include <boost/operators.hpp>          // totally_ordered

namespace dctl {
namespace successor {

// specialization for Czech draughts
template<>
class Value<rules::Czech>
:
        boost::totally_ordered< Value<rules::Czech> > // < >= > <= == !=
{
public:
        // constructors

        constexpr Value() = default;

        explicit constexpr Value(bool b) noexcept
        :
                is_with_king_{b}
        {}

        template<class U>
        explicit constexpr Value(U const& u) noexcept
        :
                Value{u.is_with_king()}
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

}       // namespace successor
}       // namespace dctl
