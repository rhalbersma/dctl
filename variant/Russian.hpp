#pragma once

namespace dctl {
namespace variant {

// http://www.shashist.ru/kodeks/kodeks2004.doc
struct Russian;

// alias for the Russian rules
typedef Russian Shashki;

}       // namespace variant
}       // namespace dctl

#include "../rules/Russian.hpp"
#include "../capture/Russian.hpp"
#include "../notation/Russian.hpp"
