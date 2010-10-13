#pragma once
#include "Reversible/Pieces.h"
#include "../Containers/Generic/VectorBucket.h"
#include "../Utilities/IntegerTypes.h"

class Move
{
public:
        // typedefs
        typedef VectorBucket<Pieces, 128> List;
        typedef VectorBucket<size_t, 128> Order;
        typedef VectorBucket<size_t, 128> Sequence;

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

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "Move.hpp"
