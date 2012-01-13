#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/vector.hpp>
#include "Predicates.hpp"
#include "../utility/Int2Type.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ = Board::ExternalGrid::size - 1
>
struct Init
{
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Test<Predicate, Board, ArgsTuple, SQ    >::value) ^
                (Init<Predicate, Board, ArgsTuple, SQ - 1>::value)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple
>
struct Init<Predicate, Board, ArgsTuple, 0>
{
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Test<Predicate, Board, ArgsTuple, 0>::value)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ
>
struct Test
{
        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Predicate<Board, ArgsTuple, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0)
        );
};

template<typename Board>
struct init_squares
{
        typedef boost::mpl::vector<> ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_square, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color>
struct init_initial
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>
        > ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_initial, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color, int Row>
struct init_row_mask
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>,
                boost::mpl::integral_c<int, Row>
        > ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_row_mask, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color, int Column>
struct init_col_mask
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>,
                boost::mpl::integral_c<int, Column>
        > ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_col_mask, Board, ArgsTuple>::value)
        );
};

template<typename Board, int Group>
struct init_jump_group
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<int, Group>
        > ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_jump_group, Board, ArgsTuple>::value)
        );
};

template<typename Board, int Index>
struct init_jump_start
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<int, Index>
        > ArgsTuple;

        // NOTE: parenthesized multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (Init<is_jump_start, Board, ArgsTuple>::value)
        );
};

}       // namespace board
}       // namespace dctl
