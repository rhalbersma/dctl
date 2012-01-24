#pragma once

namespace dctl {

template<typename X, typename G>
struct rotate
:
        rotate<typename X::type, typename G::type>
{};

template<typename G>
struct inverse
:
        inverse<typename G::type>
{};

template<typename G>
struct mirror_up
:
        mirror_up<typename G::type>
{};

template<typename G>
struct mirror_down
:
        mirror_down<typename G::type>
{};

}       // namespace dctl
