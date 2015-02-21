#pragma once
#include <dctl/search/bound.hpp>
#include <dctl/search/score.hpp>

namespace dctl {
namespace search {

class Transposition
{
         int value_ : 16;       // [-32K, +32K]
         int type_  :  2;       // 4 types
         int depth_ :  7;       // 128 ply
         int move_  :  7;       // 128 moves

public:
        Transposition(int v, int t, int d, int m)
        :
                value_(v),
                type_(t),
                depth_(d),
                move_(m)
        {}

        Transposition()
        :
                value_(0),
                type_(0),
                depth_(0),
                move_(0)
        {}

        // value
        int value() const
        {
                return value_;
        }

        // refined value
        int refined_value(int score) const
        {
                return is_cutoff(score) ? value() : score;
        }

        // upper or lower bound, or exact
        int type() const
        {
                return type_;
        }

        // remaining depth to search
        int depth() const
        {
                return depth_;
        }

        // index of the best move
        int move() const
        {
                return move_;
        }

        // check for a cutoff against a non-null window
        bool is_cutoff(int alpha, int beta) const
        {
                return is_fail_low(alpha) || is_fail_high(beta);
        }

        // check for a cutoff against a null window
        bool is_cutoff(int beta) const
        {
                return is_cutoff(beta - 1, beta);
        }

        // check for a fail low cutoff
        bool is_fail_low(int alpha) const
        {
                return has_upper_bound() && value() <= alpha;
        }

        // check for a fail high cutoff
        bool is_fail_high(int beta) const
        {
                return has_lower_bound() && beta <= value();
        }

        // check for a best move
        bool has_move() const
        {
                return move() != no_move();
        }

        // output
        std::string print_move() const
        {
                std::stringstream sstr;
                if (has_move())
                        sstr << move();
                else
                        sstr << "...";
                return sstr.str();
        }

        // modifiers

        static int no_move()
        {
                return (1 << 7) - 1;
        }

private:
        // implementation
        bool has_lower_bound() const
        {
                return (type() & Bound::lower) != 0;
        }

        bool has_upper_bound() const
        {
                return (type() & Bound::upper) != 0;
        }

        bool is_lower_bound() const
        {
                return type() == Bound::lower;
        }

        bool is_upper_bound() const
        {
                return type() == Bound::upper;
        }

        bool is_exact_value() const
        {
                return type() == Bound::exact;
        }
};

}       // namespace search
}       // namespace dctl
