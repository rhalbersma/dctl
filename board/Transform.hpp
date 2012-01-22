#pragma once

namespace dctl {

template<typename T, typename A>
struct rotate
:
        rotate<typename T::type, typename A::type>
{};

template<typename T>
struct inverse
:
        inverse<typename T::type>
{};

template<typename T>
struct mirror_up
:
        mirror_up<typename T::type>
{};

template<typename T>
struct mirror_down
:
        mirror_down<typename T::type>
{};

}       // namespace dctl
