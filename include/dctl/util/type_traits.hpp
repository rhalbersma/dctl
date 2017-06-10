#pragma once

namespace dctl::core {

template<class T> using   action_t = typename T::  action_type;
template<class T> using    board_t = typename T::   board_type;
template<class T> using    index_t = typename T::   index_type;
template<class T> using position_t = typename T::position_type;
template<class T> using    rules_t = typename T::   rules_type;
template<class T> using      set_t = typename T::     set_type;
template<class T> using   square_t = typename T::  square_type;
template<class T> using    state_t = typename T::   state_type;
template<class T> using    value_t = typename T::   value_type;

}       // namespace dctl::core
