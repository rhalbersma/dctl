#pragma once
#include "Pieces.h"
#include "../Rules/Rules.h"
#include "../Containers/VectorBucket.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/Ply.h"
#include "../Utilities/TemplateTricks.h"

class Move
{
public:
        // typedefs
        typedef VectorBucket<Pieces, MOVE_MAX> List;
        typedef VectorBucket<size_t, MOVE_MAX> Order;
        typedef VectorBucket<size_t, PLY_MAX> Sequence;
        enum Type { JUMPS, MOVES };

        // predicate
        template<typename> static bool non_unique_back(const List&);

        // modifiers
        template<bool> static void push_back(List&, BitBoard);                                          // king move
        template<bool> static void push_back(List&, BitBoard, BitBoard);                                // man move
        template<bool, typename> static void push_back(List&, BitBoard, BitBoard, BitBoard);            // king capture
        template<bool, typename> static void push_back(List&, BitBoard, BitBoard, BitBoard, BitBoard);  // man capture

private:
        // implementation
        static bool non_unique_back(const List&, Int2Type<false>);
        static bool non_unique_back(const List&, Int2Type<true>);

        template<bool> static void init(Pieces&, BitBoard);                                             // king move
        template<bool> static void init(Pieces&, BitBoard, BitBoard);		                        // man move
        template<bool, typename> static void init(Pieces&, BitBoard, BitBoard, BitBoard);               // king capture
        template<bool, typename> static void init(Pieces&, BitBoard, BitBoard, BitBoard, BitBoard);     // man capture

        // pre-conditions for initialization

        // tag dispatching based on promotion condition
        template<typename> static bool promotion_en_passant(BitBoard, BitBoard);                        // Russian draughts
        static bool promotion_en_passant(BitBoard, BitBoard, Int2Type<PROMOTE_BR>);
        static bool promotion_en_passant(BitBoard, BitBoard, Int2Type<PROMOTE_EP>);

        // tag dispatching on capture removal
        template<typename> static bool sequential_capture_removal(BitBoard, BitBoard);                  // Thai draughts
        static bool sequential_capture_removal(BitBoard, BitBoard, Int2Type<REMOVE_1>);        
        static bool sequential_capture_removal(BitBoard, BitBoard, Int2Type<REMOVE_N>);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Move.hpp"
