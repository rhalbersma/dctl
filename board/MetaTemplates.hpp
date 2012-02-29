#pragma once
#include <boost/mpl/bitxor.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/mpl/vector.hpp>
#include "Predicates.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace board {

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ
>
struct Test
:
        boost::mpl::eval_if<
                Predicate<Board, ArgsTuple, SQ>,
                boost::mpl::shift_left<
                        boost::mpl::integral_c<BitBoard, 1>,
                        boost::mpl::int_< square_to_bit<Board, SQ>::value >
                >,
                boost::mpl::integral_c<BitBoard, 0>
        >
{};

template<template<typename, typename, int> class, typename Board, typename, int = Board::ExternalGrid::size - 1> struct Init;

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple
>
struct Init<Predicate, Board, ArgsTuple, 0>
:
        Test<Predicate, Board, ArgsTuple, 0>
{};

template
<
        template<typename, typename, int> class Predicate, 
        typename Board, 
        typename ArgsTuple, 
        int SQ
>
struct Init
:
        boost::mpl::bitxor_<
                Test<Predicate, Board, ArgsTuple, SQ    >,
                Init<Predicate, Board, ArgsTuple, SQ - 1>
        >
{};

template<typename Board>
struct init_squares
:
        Init< is_square, Board, boost::mpl::vector<> >
{};

template<typename Board, bool Color>
struct init_initial
:
        Init< 
                is_initial, Board, boost::mpl::vector< 
                        boost::mpl::integral_c<bool, Color> 
                > 
        >        
{};

template<typename Board, bool Color, int Row>
struct init_row_mask
:
        Init<
                is_row_mask, Board, boost::mpl::vector<
                        boost::mpl::integral_c<bool, Color>,
                        boost::mpl::integral_c<int, Row>
                >
        >
{};

template<typename Board, bool Color, int Column>
struct init_col_mask
:
        Init<
                is_col_mask, Board, boost::mpl::vector<
                        boost::mpl::integral_c<bool, Color>,
                        boost::mpl::integral_c<int, Column>
                >
        >
{};

template<typename Board, int Group>
struct init_jump_group
:
        Init< is_jump_group, Board, boost::mpl::vector< boost::mpl::integral_c<int, Group> > >
{};

template<typename Board, int Index>
struct init_jump_start
:
        Init< is_jump_start, Board, boost::mpl::vector< angle<Index> > >
{};

}       // namespace board
}       // namespace dctl
