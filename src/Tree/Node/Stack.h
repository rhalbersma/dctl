#pragma once
#include <cstddef>
#include "Pieces.h"
#include "../../Utilities/IntegerTypes.h"
#include "../../Utilities/Ply.h"
#include "../../Utilities/TemplateTricks.h"
#include "../../Utilities/VectorArray.h"

namespace tree {
namespace node {

// typedefs
enum Type { JUMPS, MOVES };
typedef VectorArray<size_t, MAX_PLY> Sequence;
typedef VectorArray<size_t, MAX_MOVES> Order;
typedef VectorArray<Pieces, MAX_MOVES> Stack;

// element access
Stack::reference top(Stack&);
Stack::const_reference top(const Stack&);

// predicates
template<typename> bool non_unique_top(const Stack&);
bool non_unique_top(const Stack&, Int2Type<false>);
bool non_unique_top(const Stack&, Int2Type<true >);

// modifiers
template<bool> void push(BitBoard, Stack&);                                             // add a king move
template<bool> void push(BitBoard, BitBoard, Stack&);                                   // add a man move
template<bool, typename> void push(BitBoard, BitBoard, BitBoard, Stack&);               // add a king capture
template<bool, typename> void push(BitBoard, BitBoard, BitBoard, BitBoard, Stack&);     // add a man capture
void pop(Stack&);                                                                       // remove the last move  

}       // namespace node
}       // namespace tree

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Stack.hpp"
