#pragma once
#include "Enum.hpp"

namespace dctl {

namespace variant { struct Italian; }

namespace rules {

// capture mechanics
template<typename> struct is_pawns_jump_kings;
template<> struct is_pawns_jump_kings<variant::Italian>         { enum { value = false   }; };

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::Italian>      { enum { value = true    }; };

template<typename> struct is_qualified_majority;
template<> struct is_qualified_majority<variant::Italian>       { enum { value = true    }; };

template<typename> struct is_relative_king_precedence;
template<> struct is_relative_king_precedence<variant::Italian> { enum { value = true    }; };

}       // namespace rules
}       // namespace dctl
