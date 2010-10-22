#pragma once
#include <string>
#include <functional>

struct PlusOne;

template<typename>
struct write_square_layout: public std::unary_function<void, std::string>
{
        std::string operator()(void) const;
        template<typename F> std::string operator()(F) const;
};

struct Identity;

template<typename>
struct write_bit_layout: public std::unary_function<void, std::string>
{
        std::string operator()(void) const;
	template<typename F> std::string operator()(F) const;
};

template<typename>
struct is_end_row: public std::unary_function<bool, size_t>
{
        bool operator()(size_t) const;
};

template<typename>
struct is_indent_row: public std::unary_function<bool, size_t>
{
        bool operator()(size_t) const;
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "BoardIO.hpp"
