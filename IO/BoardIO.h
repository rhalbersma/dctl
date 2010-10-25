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

template<typename> bool is_end_row(size_t);
template<typename> bool is_indent_row(size_t);

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "BoardIO.hpp"
