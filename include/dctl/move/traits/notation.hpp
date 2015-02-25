#include <type_traits>          // integral_constant
#include <dctl/board/types.hpp> // International
#include <dctl/rules.hpp>       // International, Checkers, Pool, Italian

namespace dctl {

enum { algebraic = 1, numeric = 2 };

namespace move {
namespace traits {

template<class Rules, class Board>
struct notation
:
        std::integral_constant<int, algebraic>
{};

template<class Rules>
struct notation<Rules, board::International>
:
        std::integral_constant<int, numeric>
{};

template<class Rules>
struct notation<Rules, board::Canadian>
:
        std::integral_constant<int, numeric>
{};

template<>
struct notation<checkers::Rules, board::Checkers>
:
        std::integral_constant<int, numeric>
{};

template<>
struct notation<pool::Rules, board::Checkers>
:
        std::integral_constant<int, numeric>
{};

template<>
struct notation<italian::Rules, board::Roman>
:
        std::integral_constant<int, numeric>
{};

template<>
struct notation<spanish::Rules, board::Roman>
:
        std::integral_constant<int, numeric>
{};

template<>
struct notation<thai::Rules, board::Checkers>
:
        std::integral_constant<int, numeric>
{};

}       // namespace traits
}       // namespace move
}       // namespace dctl
