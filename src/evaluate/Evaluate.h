#pragma once

namespace dctl { 
        
template<typename> class Position;

class Evaluate
{
public:
        template<typename B> static int evaluate(const Position<B>& p);
        template<bool, typename B> static int delta_evaluate(const Position<B>& p);
        template<bool, typename B> static int evaluate(const Position<B>& p);
        template<typename B> static void print_break_down(const Position<B>& p);

private:
        template<bool, typename B> static int delta_material(const Position<B>& p);
        template<bool, typename B> static int delta_tempo(const Position<B>& p);
        template<bool, typename B> static int delta_center(const Position<B>& p);
        template<bool, typename B> static int delta_balance(const Position<B>& p);
        template<bool, typename B> static int delta_mobility(const Position<B>& p);

        template<bool, typename B> static int material(const Position<B>& p);
        template<bool, typename B> static int tempo(const Position<B>& p);
        template<bool, typename B> static int center(const Position<B>& p);
        template<bool, typename B> static int balance(const Position<B>& p);
        template<bool, typename B> static int mobility(const Position<B>& p);
};

}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "Evaluate.hpp"
