#pragma once

namespace dctl {
namespace board {

template<typename T> 
struct is_orthogonal
:
        is_orthogonal<typename T::type>
{};

template<typename T> 
struct is_diagonal
:
        is_diagonal<typename T::type>
{};

template<typename T> 
struct is_up
:
        is_up<typename T::type>
{};

template<typename T> 
struct is_down
:
        is_down<typename T::type>
{};

template<typename T> 
struct is_left
:
        is_left<typename T::type>
{};

template<typename T> 
struct is_right
:
        is_right<typename T::type>
{};

template<typename T> 
struct is_positive
:
        is_positive<typename T::type>
{};

template<typename T> 
struct is_negative
:
        is_negative<typename T::type>
{};

}       // namespace board
}       // namespace dctl
