#pragma once
#include "Reversible/Pieces.h"
#include "../Containers/Generic/VectorBucket.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/Ply.h"

class Move
{
public:
        // typedefs
        typedef VectorBucket<Pieces, MOVE_MAX> List;
        typedef VectorBucket<size_t, MOVE_MAX> Order;
        typedef VectorBucket<size_t, PLY_MAX> Sequence;

        // predicate
        static bool unique_back(const List&);

        // modifiers
        template<bool> static void push_back(List&, BitBoard);                                  // add a king move
        template<bool> static void push_back(List&, BitBoard, BitBoard);                        // add a man move
        template<bool> static void push_back(List&, BitBoard, BitBoard, BitBoard);              // add a king capture
        template<bool> static void push_back(List&, BitBoard, BitBoard, BitBoard, BitBoard);    // add a man capture

private:
        // implementation
        template<bool> static void init(Pieces&, BitBoard);                                     // king move
        template<bool> static void init(Pieces&, BitBoard, BitBoard);		                // man move
        template<bool> static void init(Pieces&, BitBoard, BitBoard, BitBoard);                 // king capture
        template<bool> static void init(Pieces&, BitBoard, BitBoard, BitBoard, BitBoard);       // man capture

        // pre-conditions for initialization
        static bool promotion_en_passant(BitBoard, BitBoard);                                   // Russian draughts
        static bool coup_thai(BitBoard, BitBoard);                                              // Thai draughts
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Move.hpp"
