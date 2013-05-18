#pragma once
#include <tuple>                        // tuple
#include <vector>                       // vector

namespace dctl {
namespace variadic {
namespace detail {

// the lambda is fully bound with one element from each of the vectors in the tuple
template<typename F>
inline void cross(F f)
{
        // evaluate the lambda
        f();
}

// "peal off" the first vector from the remaining tuple of vectors
template<typename F, typename Head, typename... Tail>
inline void cross(F f, std::vector<Head> const& head, std::vector<Tail> const&... tail)
{
        // "peal off" the elements from the first of the remaining vectors
        // NOTE: the recursion will effectively generate the multiple nested for-loops
        for (auto const& h: head) {
                // bind the first free variable in the lambda, and
                // keep one free variable for each of the remaining vectors
                detail::cross(
                        [&](Tail const&... elems){ f(h, elems...); },
                        tail...
                );
        }
}

}       // namespace detail

// convert a variadic tuple of vectors to a vector of tuples representing the Cartesian product
// freely adapted from this StackOverflow answer: http://stackoverflow.com/a/13841673/819272
template<typename... Args>
std::vector< std::tuple<Args...> > cartesian_product(std::vector<Args> const&... args)
{
        std::vector< std::tuple<Args...> > result;
        detail::cross(
                 [&](Args const&... elems){ result.emplace_back(elems...); },
                 args...
        );
        return result;
}

}       // namespace variadic
}       // namespace dctl
