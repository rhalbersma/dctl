#pragma once

namespace dctl {

template<class T>
using board_type_t = typename T::board_type;

template<class T>
using rules_type_t = typename T::rules_type;

template<class T>
using set_type_t = typename T::set_type;

template<class T>
using type_t = typename T::type;

template<class T>
using value_type_t = typename T::value_type;

}       // namespace dctl
