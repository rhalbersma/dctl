#pragma once

namespace dctl {
namespace variant {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian {};

// aliases for the rules of checkers
typedef Russian Shashki;

}       // namespace variant
}       // namespace dctl

#include "../rules/Russian.h"
#include "../capture/Russian.hpp"
#include "../notation/Russian.hpp"
