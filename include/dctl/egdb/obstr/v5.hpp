#pragma once
#include <xstd/bitset/intrinsic.hpp>
#include <cstdint>
#include <iterator>

namespace dctl {
namespace egdb {
namespace obstr {
namespace v5 {

using BB = uint64_t;

inline
int mirror(int sq)
{
        return 63 - sq;
}

inline
int LSB(BB x)
{
        return xstd::intrinsic::bsfnz(x);
}

inline
int MSB(BB x)
{
        return xstd::intrinsic::bsrnz(x);
}

inline
BB front_inserted(BB src, BB pat)
{
        BB dst(0);
        while (src) {
                auto sq_i = 1ULL << MSB(src);
                src ^= sq_i;
                auto const behind = sq_i - 1;
                auto skipcount = xstd::intrinsic::popcount(pat & behind);
                auto infront = ~(pat | behind);
                while (skipcount--)
                        infront &= infront - 1;
                dst |= 1ULL << LSB(infront);
        }
        return dst;
}

inline
BB front_erased(BB src, BB pat)
{
        BB dst(0);
        while (src) {
                int sq_i = LSB(src);
                src ^= 1ULL << sq_i;
                auto const skipped = xstd::intrinsic::popcount(pat & ((1ULL << sq_i) - 1ULL));
                dst |= 1ULL << (sq_i - skipped);
        }
        return dst;
}

}       // namespace v5
}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
