#pragma once
#include <string>

template<typename>
struct write_square_layout
{
	std::string operator()(void);
};

template<typename>
struct write_bit_layout
{
	std::string operator()(void);
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "BoardIO.hpp"