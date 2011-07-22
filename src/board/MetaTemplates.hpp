#include "Predicates.h"
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace board {

template<typename Board, int SQ>
class init_squares
{
private:
        static const BitBoard mask = is_square<Board, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_squares<Board, SQ-1>::value;
};

template<typename Board>
class init_squares<Board, 0>
{
private:
        static const BitBoard mask = is_square<Board, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

template<typename Board, bool C, int SQ>
class init_initial
{
private:
        static const BitBoard mask = is_initial<Board, C, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_initial<Board, C, SQ-1>::value;
};

template<typename Board, bool C>
class init_initial<Board, C, 0>
{
private:
        static const BitBoard mask = is_initial<Board, C, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

template<typename Board, bool C, int ROW, int SQ>
class init_row_mask
{
private:
        static const BitBoard mask = is_row_mask<Board, C, ROW, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_row_mask<Board, C, ROW, SQ-1>::value;
};

template<typename Board, bool C, int ROW>
class init_row_mask<Board, C, ROW, 0>
{
private:
        static const BitBoard mask = is_row_mask<Board, C, ROW, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

template<typename Board, bool C, int COL, int SQ>
class init_col_mask
{
private:
        static const BitBoard mask = is_col_mask<Board, C, COL, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_col_mask<Board, C, COL, SQ-1>::value;
};

template<typename Board, bool C, int COL>
class init_col_mask<Board, C, COL, 0>
{
private:
        static const BitBoard mask = is_col_mask<Board, C, COL, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

template<typename Board, int G, int SQ>
class init_man_jump_group
{
private:
        static const BitBoard mask = is_man_jump_group<Board, G, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_man_jump_group<Board, G, SQ-1>::value;
};

template<typename Board, int G>
class init_man_jump_group<Board, G, 0>
{
private:
        static const BitBoard mask = is_man_jump_group<Board, G, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

template<typename Board, int I, int SQ>
class init_jumpable
{
private:
        static const BitBoard mask = is_jumpable<Board, I, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0;

public:
        static const BitBoard value = mask ^ init_jumpable<Board, I, SQ-1>::value;
};

template<typename Board, int I>
class init_jumpable<Board, I, 0>
{
private:
        static const BitBoard mask = is_jumpable<Board, I, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0;

public:
        static const BitBoard value = mask;
};

}       // namespace board
}       // namespace dctl
