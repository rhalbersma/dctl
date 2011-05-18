#pragma once
#include "Types.h"
#include "../Node/Pieces.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/Ply.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Utilities/VectorArray.h"

namespace Tree {
namespace Move {

class Stack
{
public:
        // capacity
        size_t size(void) const;
        bool empty(void) const;

        // element access
              Node::Pieces& operator[](size_t);
        const Node::Pieces& operator[](size_t) const;
              Node::Pieces& top(void);
        const Node::Pieces& top(void) const;

        // predicates
        template<typename> bool non_unique_top(void);

        // modifiers
        template<bool> void push(BitBoard);                                             // king move
        template<bool> void push(BitBoard, BitBoard);                                   // man move
        template<bool, typename> void push(BitBoard, BitBoard, BitBoard);               // king capture
        template<bool, typename> void push(BitBoard, BitBoard, BitBoard, BitBoard);     // man capture
        void pop(void);
        void clear(void);

private:
        // implementation
        bool non_unique_top(Int2Type<false>);
        bool non_unique_top(Int2Type<true >);

        // representation
        VectorArray<Node::Pieces, MOVE_MAX> vector_array_;
};

}       // namespace Move
}       // namespace Tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Stack.hpp"
