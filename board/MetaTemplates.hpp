#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Predicates.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple, 
        int SQ
>
struct test_predicate
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Predicate<Board, Tuple, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename Tuple, 
        int SQ = Board::ExternalGrid::size - 1
>
struct init_predicate
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (test_predicate<Predicate, Board, Tuple, SQ  >::value) ^
                (init_predicate<Predicate, Board, Tuple, SQ-1>::value)
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
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (test_predicate<Predicate, Board, Tuple, 0>::value)
        );
};

template<typename Board>
struct init_squares
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_square, Board, empty_tuple>::value)
        );
};

template<typename Board, bool Color>
struct init_initial
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_initial, Board, bool_tuple<Color> >::value)
        );
};

template<typename Board, bool Color, int Row>
struct init_row_mask
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_row_mask, Board, bool_int_tuple<Color, Row> >::value)
        );
};

template<typename Board, bool Color, int Column>
struct init_col_mask
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_col_mask, Board, bool_int_tuple<Color, Column> >::value)
        );
};

template<typename Board, int Group>
struct init_jump_group
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_jump_group, Board, int_tuple<Group> >::value)
        );
};

template<typename Board, int Index>
struct init_jump_start
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_jump_start, Board, int_tuple<Index> >::value)
        );
};

}       // namespace board
}       // namespace dctl
