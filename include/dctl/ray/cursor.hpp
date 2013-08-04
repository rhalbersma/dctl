#pragma once
#include <boost/operators.hpp>          // additive, unit_steppable

namespace dctl {
namespace ray {

template<int N>
class StridedCursor
:
        boost::additive      < StridedCursor<N>, int    // +, -
,       boost::unit_steppable< StridedCursor<N>         // ++, --
> >
{
        using self_type = StridedCursor;
public:
        explicit StridedCursor(int c)
        :
                cursor_ { c }
        {}

        template<int M>
        StridedCursor(StridedCursor<M> const& other)
        :
                cursor_ { other.cursor_ }
        {}

        friend int operator-(self_type const& lhs, self_type const& rhs)
        {
                return (lhs.cursor_ - rhs.cursor_) / N;
        }

        // operator+(self_type, int) provided by boost::additive
        self_type& operator+=(int n)
        {
                cursor_ += N * n;
                return *this;
        }

        // operator-(self_type, int) provided by boost::additive
        self_type& operator-=(int n)
        {
                cursor_ -= N * n;
                return *this;
        }

        // operator++(int) provided by boost::unit_steppable
        self_type& operator++()
        {
                cursor_ += N;
                return *this;
        }

        // operator--(int) provided by boost::unit_steppable
        self_type& operator--()
        {
                cursor_ -= N;
                return *this;
        }

        operator int() const
        {
                return cursor_;
        }

private:
        int cursor_;
};

}       // namespace ray
}       // namespace dctl
