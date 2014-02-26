#pragma once
#include <utility>                      // forward

namespace dctl {
namespace util {

// the Standard mandates that mutable lambdas have a non-const operator()
// the mutating algorithm boost::transform() requires a const operator()
// force_const_call_t wraps a mutable lambda as a mutable data member
// inside a function object with a const operator()

template<class MutableLambda>
struct force_const_call_t
{
        mutable MutableLambda f;

        template<class... Args>
        auto operator()(Args&&... args) const
        {
                return f(std::forward<Args>(args)...);
        }
};

template<class MutableLambda>
force_const_call_t<MutableLambda> force_const_call(MutableLambda&& f)
{
        return { std::forward<MutableLambda>(f) };
}

}
}
