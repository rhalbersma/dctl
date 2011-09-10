#pragma once

namespace dctl {

namespace variant { struct International; }   

namespace rules {

// capture precedence
template<typename> struct is_majority_precedence;
template<> struct is_majority_precedence<variant::International> { enum { value = true  }; };

}       // namespace rules
}       // namespace dctl
