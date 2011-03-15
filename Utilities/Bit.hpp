#include "BitTricks.h"
#include <cassert>

namespace Bit {

template<typename T>
bool is_zero(T b)
{
        return b == 0;
}

template<typename T>
bool is_single(T b)
{
        return !(is_zero(b) || is_multiple(b));
}

template<typename T>
bool is_double(T b)
{
        return is_single(except_lowest(b));
}

template<typename T>
bool is_multiple(T b)
{
        return !is_zero(except_lowest(b));
}

template<typename T>
bool is_within(T a, T b)
{
        return is_zero(a & ~b);
}

template<typename T>
bool is_exclusive(T a, T b)
{
        return is_zero(a & b);
}

template<typename T>
T get_lowest(T b)
{
        return b & (0 - b);
}

template<typename T>
T except_lowest(T b)
{
        return b & (b - 1);
}

template<typename T>
size_t scan_forward(T b)
{
        return index(get_lowest(b));
}

template<typename T>
size_t index(T b)
{
	assert(is_single(b));
        return index_DeBruijn(b);
}

template<typename T>
size_t count(T b)
{
        return count_Kernighan(b);
}

template<bool Sign, typename T>
T flood_fill(T generator, T propagator, size_t dir)
{
        return fill_loop<Sign>(generator, propagator, dir);
}

template<typename T>
void clear_lowest(T& b)
{
        b &= b - 1;
}

}       // namespace Bit
