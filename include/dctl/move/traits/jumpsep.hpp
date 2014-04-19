#pragma once
#include <type_traits>                  // integral_constant
#include <dctl/rules/variants.hpp>      // Czech, Russian, Thai

namespace dctl {
namespace move {
namespace traits {

template<class Rules>
struct jumpsep
:
        std::integral_constant<char, 'x'>
{};

template<>
struct jumpsep<rules::Czech>
:
        std::integral_constant<char, ':'>
{};

template<>
struct jumpsep<rules::Russian>
:
        std::integral_constant<char, ':'>
{};

template<>
struct jumpsep<rules::Thai>
:
        std::integral_constant<char, '-'>
{};

}       // namespace traits
}       // namespace move
}       // namespace dctl
