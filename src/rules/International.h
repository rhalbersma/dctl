#pragma once

namespace dctl {
namespace rules {

// http://www.fmjd.org/docs/Annex%201%20official%20FMJD%20rules%20of%20international%20draughts.doc
struct International {};   

// aliases for the international rules
typedef International Brazilian;        //  8x8  board
typedef International Polish;           // 10x10 board
typedef International Canadian;         // 12x12 board

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<International> { enum { value = true  }; };

}       // namespace rules
}       // namespace dctl
