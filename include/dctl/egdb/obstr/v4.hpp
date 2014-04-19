#pragma once
#include <cstdint>
#include <iterator>
#include <dctl/bit/detail/intrinsic.hpp>

namespace dctl {
namespace egdb {
namespace obstr {
namespace v4 {

using BB = uint64_t;

inline
int mirror(int sq)
{
        return 63 - sq;
}

inline
int LSB(BB x)
{
        return bit::intrinsic::bsfnz(x);
}

inline
int MSB(BB x)
{
        return bit::intrinsic::bsrnz(x);
}

inline
BB front_inserted(BB src, BB pat)
{
        BB dst(0);
        int skipped = 0;
        while (src) {
                int sq_i = LSB(src);
                while (pat && LSB(pat) <= sq_i + skipped) {
                        pat ^= 1ULL << LSB(pat);
                        ++skipped;
                }
                dst |= 1ULL << (sq_i + skipped);
                src ^= 1ULL << sq_i;
         }
        return dst;
}

inline
BB back_inserted(BB src, BB pat)
{
        BB dst(0);
        int skipped = 0;
        while (src) {
                int sq_i = LSB(src);
                while (pat && MSB(pat) >= mirror(sq_i) - skipped) {
                        pat ^= 1ULL << MSB(pat);
                        ++skipped;
                }
                dst |= 1ULL << (mirror(sq_i) - skipped);
                src ^= 1ULL << sq_i;
        }
        return dst;
}

inline
BB front_erased(BB src, BB pat)
{
        BB dst(0);
        int skipped = 0;
        while (src) {
                int sq_i = LSB(src);
                while (pat && LSB(pat) < sq_i) {
                        pat ^= 1ULL << LSB(pat);
                        ++skipped;
                }
                dst |= 1ULL << (sq_i - skipped);
                src ^= 1ULL << sq_i;
        }
        return dst;
}

inline
BB back_erased(BB src, BB pat)
{
        BB dst(0);
        int skipped = 0;
        while (src) {
                int sq_i = MSB(src);
                while (pat && MSB(pat) > sq_i) {
                        pat ^= 1ULL << MSB(pat);
                        ++skipped;
                }
                dst |= 1ULL << mirror(sq_i + skipped);
                src ^= 1ULL << sq_i;
        }
        return dst;
}

}       // namespace v4
}       // namespace obstr
}       // namespace egdb
}       // namespace dctl
