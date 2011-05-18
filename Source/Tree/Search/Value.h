#pragma once
#include <string>

namespace Tree {
namespace Search {
namespace Value {

// predicates
bool is_loss(int);
bool is_win(int);
bool is_infinity(int);
bool is_heuristic(int);
bool is_finite(int);

// views
int loss(int);
int draw(void);
int win(int);
int infinity(void);

// modifiers
int squeeze(int);
int stretch(int);

std::string print(int);

}       // namespace Value
}       // namespace Search
}       // namespace Tree
