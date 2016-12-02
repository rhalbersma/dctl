#pragma once
#include <iterator>

namespace dctl {
namespace egdb {
namespace obstr {
namespace v1 {

template<class Range>
auto front_inserted(Range const& src, Range const& pat)
{
        Range dst;
        auto first = begin(~pat);
        auto sq_prev = 0;
        for (auto const& sq_i : src) {
                while (sq_prev != sq_i) {
                        ++first;
                        ++sq_prev;
                }
                dst.insert(*first);
        }
        return dst;
}

template<class Range>
auto back_inserted(Range const& src, Range const& pat)
{
        Range dst;
        auto first = rbegin(~pat);
        auto sq_prev = 0;
        for (auto const& sq_i : src) {
                while (sq_prev != sq_i) {
                        ++first;
                        ++sq_prev;
                }
                dst.insert(*first);
        }
        return dst;
}

template<class Range>
auto front_erased(Range const& src, Range const& pat)
{
        Range dst;
        auto first = begin(~pat);
        auto sq_i = 0;
        for (auto const& sq : src) {
                while (*first != sq) {
                        ++first;
                        ++sq_i;
                }
                dst.insert(sq_i);
        }
        return dst;
}

template<class Range>
auto back_erased(Range const& src, Range const& pat)
{
        Range dst;
        auto first = rbegin(~pat);
        auto sq_i = 0;
        for (auto psq = rbegin(src); psq != rend(src); ++psq) {
                while (*first != *psq) {
                        ++first;
                        ++sq_i;
                }
                dst.insert(sq_i);
        }
        return dst;
}

}       // namespace v1
}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
