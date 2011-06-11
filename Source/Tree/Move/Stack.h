#pragma once
#include "Types.h"
#include "../Node/Pieces.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/Ply.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Utilities/VectorArray.h"

namespace tree {
namespace move {

class Stack
{
public:
        // capacity
        size_t size(void) const;
        bool empty(void) const;

        // element access
              node::Pieces& operator[](size_t);
        const node::Pieces& operator[](size_t) const;
              node::Pieces& top(void);
        const node::Pieces& top(void) const;

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
        VectorArray<node::Pieces, MOVE_MAX> vector_array_;
};

}       // namespace move
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Stack.hpp"
