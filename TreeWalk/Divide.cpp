#include "Divide.h"
#include <iostream>

void TreeWalk::Divide::print_move(const std::string& move, size_t i)
{
        std::cout << std::setw(2) << (i + 1) << "." << move << " ";
}

void TreeWalk::Divide::summary(NodeCount leafs)
{
        std::cout << std::endl << "Total leafs: " << leafs << std::endl;
        std::cout << std::endl;
}
