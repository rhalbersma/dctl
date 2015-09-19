#pragma once

namespace dctl {

template<class T>
using state_t = typename T::state_type;

template<class T>
using board_t = typename T::board_type;

template<class T>
using rules_t = typename T::rules_type;

template<class T>
using set_t = typename T::set_type;

template<class T>
using move_t = typename T::move_type;

template<class T>
using type_t = typename T::type;

template<class T>
using value_t = typename T::value_type;

}       // namespace dctl
