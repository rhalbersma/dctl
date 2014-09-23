#pragma once
#include <type_traits>                  // integral_constant
#include <dctl/rules.hpp>      // Czech, Russian, Thai

namespace dctl {
namespace move {
namespace traits {

template<class Rules>
struct jumpsep
:
        std::integral_constant<char, 'x'>
{};

template<>
struct jumpsep<czech::Rules>
:
        std::integral_constant<char, ':'>
{};

template<>
struct jumpsep<russian::Rules>
:
        std::integral_constant<char, ':'>
{};

template<>
struct jumpsep<thai::Rules>
:
        std::integral_constant<char, '-'>
{};

}       // namespace traits
}       // namespace move
}       // namespace dctl
