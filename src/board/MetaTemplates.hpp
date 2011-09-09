#include <boost/config.hpp>
#include "Predicates.h"
#include "../utils/IntegerTypes.h"
#include "../utils/TemplateTricks.h"

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple, 
        int SQ
>
struct init_predicate
{
        BOOST_STATIC_CONSTANT(auto, value = 
                ((Predicate<Board, Tuple, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0) ^ 
                init_predicate<Predicate, Board, Tuple, SQ-1>::value)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple
>
struct init_predicate<Predicate, Board, Tuple, 0>
{
        BOOST_STATIC_CONSTANT(auto, value = 
                (Predicate<Board, Tuple, 0>::value? (BitBoard(1) << square_to_bit<Board, 0>::value) : 0)
        );
};

template<typename Board>
struct init_squares
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_square, Board, empty_tuple>::value)
        );
};

template<typename Board, bool C>
struct init_initial
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_initial, Board, bool_tuple<C> >::value)
        );
};

template<typename Board, bool C, int row>
struct init_row_mask
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_row_mask, Board, bool_int_tuple<C, row> >::value)
        );
};

template<typename Board, bool C, int col>
struct init_col_mask
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_col_mask, Board, bool_int_tuple<C, col> >::value)
        );
};

template<typename Board, int G>
struct init_man_jump_group
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_man_jump_group, Board, int_tuple<G> >::value)
        );
};

template<typename Board, int I>
struct init_jumpable
{
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_jumpable, Board, int_tuple<I> >::value)
        );
};

}       // namespace board
}       // namespace dctl
