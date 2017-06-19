#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/algo/search/bound.hpp>
#include <dctl/algo/search/score.hpp>

namespace dctl::algo {
namespace search {

class Transposition
{
         short int      value_ : 16;    // [-32K, +32K]
         short unsigned type_  :  2;    // 4 types
         short unsigned depth_ :  7;    // 128 ply
         short unsigned move_  :  7;    // 128 moves

public:
        Transposition()
        :
                value_{0},
                type_ {0},
                depth_{0},
                move_ {0}
        {}

        Transposition(int const v, unsigned const t, int const d, int const m)
        :
                value_{static_cast<short>(v)},
                type_ {static_cast<unsigned short>(t & ((1u << 2) - 1))},
                depth_{static_cast<unsigned short>(static_cast<unsigned>(d) & ((1u << 7) - 1))},
                move_ {static_cast<unsigned short>(static_cast<unsigned>(m) & ((1u << 7) - 1))}
        {}

        // value
        auto value() const
        {
                return static_cast<int>(value_);
        }

        // upper or lower bound, or exact
        auto type() const
        {
                return static_cast<unsigned>(type_);
        }

private:
        // implementation
        auto has_lower_bound() const
        {
                return (type() & Bound::lower) != 0;
        }

        auto has_upper_bound() const
        {
                return (type() & Bound::upper) != 0;
        }

        auto is_lower_bound() const
        {
                return type() == Bound::lower;
        }

        auto is_upper_bound() const
        {
                return type() == Bound::upper;
        }

        auto is_exact_value() const
        {
                return type() == Bound::exact;
        }

public:
        // remaining depth to search
        auto depth() const
        {
                return static_cast<int>(depth_);
        }

        // index of the best move
        auto move() const
        {
                return static_cast<int>(move_);
        }

        // check for a fail low cutoff
        auto is_fail_low(int alpha) const
        {
                return has_upper_bound() && value() <= alpha;
        }

        // check for a fail high cutoff
        auto is_fail_high(int beta) const
        {
                return has_lower_bound() && beta <= value();
        }

        // check for a cutoff against a non-null window
        auto is_cutoff(int alpha, int beta) const
        {
                return is_fail_low(alpha) || is_fail_high(beta);
        }

        // check for a cutoff against a null window
        auto is_cutoff(int beta) const
        {
                return is_cutoff(beta - 1, beta);
        }

        // refined value
        auto refined_value(int const score) const
        {
                return is_cutoff(score) ? value() : score;
        }

        static auto no_move()
        {
                return (1 << 7) - 1;
        }

        // check for a best move
        auto has_move() const
        {
                return move() != no_move();
        }

        // output
        auto print_move() const
        {
                std::stringstream sstr;
                if (has_move()) {
                        sstr << move();
                } else {
                        sstr << "...";
                }
                return sstr.str();
        }
};

}       // namespace search
}       // namespace dctl::algo
