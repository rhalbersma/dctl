#pragma once

namespace dctl {
namespace group {

struct IsIdempotent
{
        template<class Function, class Object>
        constexpr auto operator()(Function op, Object obj) const
        {
                return op(op(obj)) == obj;
        }
};

}       // namespace group
}       // namespace dctl
