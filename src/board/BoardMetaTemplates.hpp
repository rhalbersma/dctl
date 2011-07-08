#include "BoardPredicates.h"
#include "../Utilities/IntegerTypes.h"

namespace board {

template<typename T, int SQ>
class init_squares
{
private:
        static const BitBoard MASK = is_square<T, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_squares<T, SQ-1>::value;
};

template<typename T>
class init_squares<T, 0>
{
private:
        static const BitBoard MASK = is_square<T, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

template<typename T, bool C, int SQ>
class init_initial
{
private:
        static const BitBoard MASK = is_initial<T, C, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_initial<T, C, SQ-1>::value;
};

template<typename T, bool C>
class init_initial<T, C, 0>
{
private:
        static const BitBoard MASK = is_initial<T, C, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

template<typename T, bool C, int ROW, int SQ>
class init_row_mask
{
private:
        static const BitBoard MASK = is_row_mask<T, C, ROW, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_row_mask<T, C, ROW, SQ-1>::value;
};

template<typename T, bool C, int ROW>
class init_row_mask<T, C, ROW, 0>
{
private:
        static const BitBoard MASK = is_row_mask<T, C, ROW, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

template<typename T, bool C, int COL, int SQ>
class init_col_mask
{
private:
        static const BitBoard MASK = is_col_mask<T, C, COL, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_col_mask<T, C, COL, SQ-1>::value;
};

template<typename T, bool C, int COL>
class init_col_mask<T, C, COL, 0>
{
private:
        static const BitBoard MASK = is_col_mask<T, C, COL, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

template<typename T, int G, int SQ>
class init_man_jump_group
{
private:
        static const BitBoard MASK = is_man_jump_group<T, G, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_man_jump_group<T, G, SQ-1>::value;
};

template<typename T, int G>
class init_man_jump_group<T, G, 0>
{
private:
        static const BitBoard MASK = is_man_jump_group<T, G, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

template<typename T, int I, int SQ>
class init_jumpable
{
private:
        static const BitBoard MASK = is_jumpable<T, I, SQ>::value? (BitBoard(1) << square2bit<T, SQ>::value) : 0;

public:
        static const BitBoard value = MASK ^ init_jumpable<T, I, SQ-1>::value;
};

template<typename T, int I>
class init_jumpable<T, I, 0>
{
private:
        static const BitBoard MASK = is_jumpable<T, I, 0>::value? (BitBoard(1) << square2bit<T, 0>::value) : 0;

public:
        static const BitBoard value = MASK;
};

}       // namespace board
