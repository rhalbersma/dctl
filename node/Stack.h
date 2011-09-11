#pragma once
#include <vector>       // std::vector
#include "Move.h"
#include "../utility/IntegerTypes.h"
#include "../utility/TemplateTricks.h"

namespace dctl {

typedef std::vector<Move> Stack;

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

}       // namespace dctl

// include template definitions inside header
#include "Stack.hpp"
