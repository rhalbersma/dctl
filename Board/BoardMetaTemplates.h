#pragma once
#include "../Utilities/IntegerTypes.h"

template<typename Board, size_t = Board::NUM_BITS - 1>
struct INIT_GHOSTS;

template<typename Board, bool, size_t = Board::NUM_BITS - 1>
struct INIT_INITIAL;

template<typename Board, bool, size_t, size_t = Board::NUM_BITS - 1>
struct INIT_ROW_MASK;

template<typename Board, bool, size_t, size_t = Board::NUM_BITS - 1>
struct INIT_COL_MASK;

template<typename Board, size_t, size_t = Board::NUM_BITS - 1>
struct INIT_JUMPABLE;

template<typename Board, size_t, size_t = Board::NUM_BITS - 1>
struct INIT_MAN_JUMP_GROUP;

template<typename, size_t>
struct BIT2SQUARE;

template<typename, size_t>
struct SQUARE2BIT;

template<typename, size_t>
struct PREV_SQUARE;

template<typename, size_t, size_t>
struct FIND_BIT;

// include template definitions inside header since "export" keyword is not supported by Visual C++
#include "BoardMetaTemplates.hpp"
