#pragma once
#include <functional>                   // bind, placeholders
#include <iomanip>                      // setw
#include <sstream>                      // stringstream
#include <dctl/setup/numbers.hpp>
#include <dctl/setup/i_token.hpp>

namespace dctl {
namespace setup {

// primary template declaration
template
<
        typename Protocol,
        typename Content = typename Token<Protocol>::type
>
struct diagram;

// partial specialization declaration to write bit numbers in diagram layout
template<typename Board>
struct diagram<Board, bits>;

// partial specialization declaration to write square numbers in diagram layout
template<typename Board>
struct diagram<Board, squares>;

}       // namespace setup
}       // namespace dctl
