#pragma once
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/vector.hpp>
#include "Predicates.hpp"
#include "../utility/IntegerTypes.hpp"
#include "../utility/TemplateTricks.hpp"

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ
>
struct test_predicate
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (Predicate<Board, ArgsTuple, SQ>::value? (BitBoard(1) << square_to_bit<Board, SQ>::value) : 0)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ = Board::ExternalGrid::size - 1
>
struct init_predicate
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (test_predicate<Predicate, Board, ArgsTuple, SQ  >::value) ^
                (init_predicate<Predicate, Board, ArgsTuple, SQ-1>::value)
        );
};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple
>
struct init_predicate<Predicate, Board, ArgsTuple, 0>
{
        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value = 
                (test_predicate<Predicate, Board, ArgsTuple, 0>::value)
        );
};

template<typename Board>
struct init_squares
{
        typedef boost::mpl::vector<> ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_square, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color>
struct init_initial
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>
        > ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_initial, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color, int Row>
struct init_row_mask
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>,
                boost::mpl::integral_c<int, Row>
        > ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_row_mask, Board, ArgsTuple>::value)
        );
};

template<typename Board, bool Color, int Column>
struct init_col_mask
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<bool, Color>,
                boost::mpl::integral_c<int, Column>
        > ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_col_mask, Board, ArgsTuple>::value)
        );
};

template<typename Board, int Group>
struct init_jump_group
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<int, Group>
        > ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_jump_group, Board, ArgsTuple>::value)
        );
};

template<typename Board, int Index>
struct init_jump_start
{
        typedef boost::mpl::vector<
                boost::mpl::integral_c<int, Index>
        > ArgsTuple;

        // NOTE: parenthesize multiple argument template rvalues to avoid pre-processor argument splitting
        BOOST_STATIC_CONSTANT(auto, value =
                (init_predicate<is_jump_start, Board, ArgsTuple>::value)
        );
};

}       // namespace board
}       // namespace dctl
