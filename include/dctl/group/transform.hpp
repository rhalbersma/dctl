#pragma once

namespace dctl {
namespace group {

template<class Function, class Object>
constexpr Object apply(int n, Function op, Object obj)
{
        return n ? op(apply(n - 1, op, obj)) : obj;
}

struct IsInvolution
{
        template<class Function, class Object>
        constexpr auto operator()(Function op, Object obj) const
        {
                return apply(2, op, obj) == apply(0, op, obj);
        }
};

struct IsIdempotent
{
        template<class Function, class Object>
        constexpr auto operator()(Function op, Object obj) const
        {
                return apply(2, op, obj) == apply(1, op, obj);
        }
};

}       // namespace group
}       // namespace dctl
