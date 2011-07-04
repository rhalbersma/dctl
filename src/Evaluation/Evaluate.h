#pragma once

namespace tree { namespace node { template<typename> class Position; } }

class Evaluate
{
public:
        template<typename B> static int evaluate(const tree::node::Position<B>& p);
        template<bool, typename B> static int delta_evaluate(const tree::node::Position<B>& p);
        template<bool, typename B> static int evaluate(const tree::node::Position<B>& p);
        template<typename B> static void print_break_down(const tree::node::Position<B>& p);

private:
        template<bool, typename B> static int delta_material(const tree::node::Position<B>& p);
        template<bool, typename B> static int delta_tempo(const tree::node::Position<B>& p);
        template<bool, typename B> static int delta_center(const tree::node::Position<B>& p);
        template<bool, typename B> static int delta_balance(const tree::node::Position<B>& p);
        template<bool, typename B> static int delta_mobility(const tree::node::Position<B>& p);

        template<bool, typename B> static int material(const tree::node::Position<B>& p);
        template<bool, typename B> static int tempo(const tree::node::Position<B>& p);
        template<bool, typename B> static int center(const tree::node::Position<B>& p);
        template<bool, typename B> static int balance(const tree::node::Position<B>& p);
        template<bool, typename B> static int mobility(const tree::node::Position<B>& p);
};

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Evaluate.hpp"
