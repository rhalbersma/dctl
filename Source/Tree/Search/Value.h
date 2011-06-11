#pragma once
#include <string>

namespace tree {
namespace search {
namespace value {

// predicates
bool is_infinite(int);
bool is_finite(int);
bool is_mate(int);
bool is_loss(int);
bool is_win(int);

// views
int infinity(void);
int loss(int);
int win(int);
int draw(void);

// modifiers
int stretch(int);
int squeeze(int);

std::string print(int);

}       // namespace value
}       // namespace search
}       // namespace tree
