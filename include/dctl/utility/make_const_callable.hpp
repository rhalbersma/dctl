#pragma once
#include <utility>                      // forward

namespace dctl {
namespace util {

// the Standard mandates that mutable lambdas have a non-const operator()
// the mutating algorithm boost::transform() requires a const operator()
// make_const_callable_t wraps a mutable lambda as a mutable data member
// inside a function object with a const operator()

template<class MutableLambda>
struct make_const_callable_t
{
        mutable MutableLambda f;

        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return f(std::forward<Args>(args)...);
        }
};

template<class MutableLambda>
make_const_callable_t<MutableLambda> make_const_callable(MutableLambda&& f)
{
        return { std::forward<MutableLambda>(f) };
}

}       // namespace util
}       // namespace dctl
