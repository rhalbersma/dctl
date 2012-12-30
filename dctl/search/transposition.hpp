#pragma once
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include <dctl/search/bound.hpp>
#include <dctl/search/score.hpp>
#include <dctl/utility/int.hpp>

namespace dctl {
namespace search {

struct Transposition
{
public:
        // structors
        Transposition()
        :
                value_(0),
                rest_(0)
        {
        }

        Transposition(int v, int t, int d, int m)
        :
                value_(static_cast<int16_t>(v)),
                rest_(0)
        {
                rest_ ^= (t &  TYPE_MASK) <<  TYPE_SHIFT;
                rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
                rest_ ^= (m &  MOVE_MASK) <<  MOVE_SHIFT;
        }

        // value
        int value() const
        {
                return value_;
        }

        // refined value
        int refined_value(int score) const
        {
                return is_cutoff(score)? value() : score;
        }

        // upper or lower bound, or exact
        int type() const
        {
                return static_cast<int>((rest_ & (TYPE_MASK << TYPE_SHIFT)) >> TYPE_SHIFT);
        }

        // remaining depth to search
        int depth() const
        {
                return static_cast<int>((rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
        }

        // index of the best move
        int move() const
        {
                return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
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
                return has_lower_bound() && value() >= beta;
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

        void set_move(int m)
        {
                rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
                rest_ ^= (m      & MOVE_MASK) << MOVE_SHIFT;
        }

        static int no_move()
        {
                return MOVE_MASK;
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

        BOOST_STATIC_CONSTANT(auto, TYPE_BITS = 2);
        BOOST_STATIC_CONSTANT(auto, DEPTH_BITS = 7);
        BOOST_STATIC_CONSTANT(auto, MOVE_BITS = 7);

        BOOST_STATIC_CONSTANT(auto, TYPE_SHIFT = 0);
        BOOST_STATIC_CONSTANT(auto, DEPTH_SHIFT = TYPE_SHIFT + TYPE_BITS);
        BOOST_STATIC_CONSTANT(auto, MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS);

        BOOST_STATIC_CONSTANT(auto, TYPE_MASK = ((1 << TYPE_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, DEPTH_MASK = ((1 << DEPTH_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, MOVE_MASK = ((1 << MOVE_BITS) - 1));

        // representation
         int16_t value_;        // value
        uint16_t rest_;         // bound, depth and move
};

}       // namespace search
}       // namespace dctl
