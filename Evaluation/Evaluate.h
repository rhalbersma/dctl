#pragma once

namespace Tree { namespace Node { template<typename> class Position; } }

class Evaluate
{
public:
        template<typename B> static int evaluate(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int delta_evaluate(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int evaluate(const Tree::Node::Position<B>& p);
        template<typename B> static void print_break_down(const Tree::Node::Position<B>& p);

private:
        template<bool, typename B> static int delta_material(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int delta_tempo(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int delta_center(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int delta_balance(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int delta_mobility(const Tree::Node::Position<B>& p);

        template<bool, typename B> static int material(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int tempo(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int center(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int balance(const Tree::Node::Position<B>& p);
        template<bool, typename B> static int mobility(const Tree::Node::Position<B>& p);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Evaluate.hpp"
