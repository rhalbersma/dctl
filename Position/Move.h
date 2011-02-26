#pragma once
#include "Pieces.h"
#include "../Variants/Rules.h"
#include "../Utilities/VectorArray.h"
#include "../Utilities/IntegerTypes.h"
#include "../Utilities/Ply.h"
#include "../Utilities/TemplateTricks.h"

namespace Move {

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<size_t, MOVE_MAX> Order;
typedef VectorArray<size_t, PLY_MAX> Sequence;

class List
{
public:
        // predicate
        template<typename> bool non_unique_back(void);

        size_t size(void) const;
        const Pieces& operator[](size_t) const;

        bool empty(void) const;

        // modifiers
        template<bool> void push_back(BitBoard);                                          // king move
        template<bool> void push_back(BitBoard, BitBoard);                                // man move
        template<bool, typename> void push_back(BitBoard, BitBoard, BitBoard);            // king capture
        template<bool, typename> void push_back(BitBoard, BitBoard, BitBoard, BitBoard);  // man capture
        void pop_back(void);
        void clear(void);

private:
        // implementation
        bool non_unique_back(Int2Type<false>);
        bool non_unique_back(Int2Type<true>);

        // pre-conditions for initialization

        // tag dispatching based on promotion condition
        template<typename> static bool promotion_en_passant(BitBoard, BitBoard);                        // Russian draughts
        static bool promotion_en_passant(BitBoard, BitBoard, Int2Type<Variant::PROMOTE_BR>);
        static bool promotion_en_passant(BitBoard, BitBoard, Int2Type<Variant::PROMOTE_EP>);

        // tag dispatching on capture removal
        template<typename> static bool sequential_capture_removal(BitBoard, BitBoard);                  // Thai draughts
        static bool sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variant::REMOVE_1>);        
        static bool sequential_capture_removal(BitBoard, BitBoard, Int2Type<Variant::REMOVE_N>);

        VectorArray<Pieces, MOVE_MAX> move_list_;
};

}       // namespace Move

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Move.hpp"
