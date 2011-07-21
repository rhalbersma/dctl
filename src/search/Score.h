#pragma once
#include <string>

namespace dtl {
namespace search {
namespace score {

// predicates
bool is_infinite(int);
bool is_finite(int);
bool is_mate(int);
bool is_loss(int);
bool is_win(int);

// views
int infinity();
int loss_value(int);
int win_value(int);
int loss_ply(int);
int win_ply(int);
int draw();

// modifiers
int stretch(int);
int squeeze(int);

std::string print(int);

}       // namespace score
}       // namespace search
}       // namespace dtl
