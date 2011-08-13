#pragma once

namespace dctl {
namespace variant {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International {};   

// aliases for the international rules
typedef International Brazilian;        //  8x8  board
typedef International Polish;           // 10x10 board
typedef International Canadian;         // 12x12 board

}       // namespace variant
}       // namespace dctl

#include "../rules/International.h"
#include "../capture/International.h"
