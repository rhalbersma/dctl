#pragma once
#include <boost/range/adaptor/reversed.hpp>     // reversed
#include <boost/range/algorithm/find_if.hpp>    // find_if
#include <boost/range/irange.hpp>               // irange
#include <cassert>                              // assert

namespace dctl {
namespace egdb {
namespace detail {

template<class Binomial>
class UnRank
{
private:
        using T = typename Binomial::value_type;
        mutable T index;
        mutable int sq;

public:
        UnRank(T i, int s) : index{i}, sq{s} {}

        template<class T>
        auto operator()(T i) const
        {
                assert(0 <= (i - 1) && (i - 1) <= sq);
                sq = *boost::find_if(boost::irange(0, sq) | boost::adaptors::reversed, [&](auto n) {
                        return Binomial::coefficient(n, i) <= index;
                });
                index -= Binomial::coefficient(sq, i);
                return sq;
        }
};

}       // namespace detail
}       // namespace egdb
}       // namespace dctl
