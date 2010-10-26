#include "Divide.h"
#include <iostream>

void Divide::print_move(const std::string& move, size_t i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

void Divide::summary(NodeCount divide_count)
{
        std::cout << std::endl << "Total nodes: " << divide_count << std::endl;
        std::cout << std::endl;
}
