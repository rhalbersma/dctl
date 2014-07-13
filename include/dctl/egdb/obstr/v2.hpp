#pragma once
#include <dctl/cpp14/iterator.hpp>
#include <iterator>

namespace dctl {
namespace egdb {
namespace obstr {
namespace v2 {

template<class T>
auto mirror(T sq)
{
        return 63 - sq;
}

template<class Range>
auto front_inserted(Range const& src, Range const& pat)
{
        Range dst;
        auto first = begin(pat), last = end(pat);
        auto skipped = 0;
        for (auto&& sq_i : src) {
                while (first != last && *first <= sq_i + skipped) {
                        ++first;
                        ++skipped;
                }
                dst.set(sq_i + skipped);
        }
        return dst;
}

template<class Range>
auto back_inserted(Range const& src, Range const& pat)
{
        Range dst;
        auto first = rbegin(pat), last = rend(pat);
        auto skipped = 0;
        for (auto&& sq : src) {
                auto const sq_i = mirror(sq);
                while (first != last && *first >= sq_i - skipped) {
                        ++first;
                        ++skipped;
                }
                dst.set(sq_i - skipped);
        }
        return dst;
}


template<class Range>
auto front_erased(Range const& src, Range const& pat)
{
        Range dst;
        auto first = begin(pat), last = end(pat);
        auto skipped = 0;
        for (auto&& sq_i : src) {
                while (first != last && *first < sq_i) {
                        ++first;
                        ++skipped;
                }
                dst.set(sq_i - skipped);
        }
        return dst;
}

template<class Range>
auto back_erased(Range const& src, Range const& pat)
{
        Range dst;
        auto first = rbegin(pat), last = rend(pat);
        auto skipped = 0;
        for (auto psq = rbegin(src); psq != rend(src); ++psq) {
                while (first != last && *first > *psq) {
                        ++first;
                        ++skipped;
                }
                dst.set(mirror(*psq + skipped));
        }
        return dst;
}

}       // namespace v2
}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
