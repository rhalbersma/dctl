#include <cassert>
#include <sstream>
#include "Entry.h"
#include "Score.h"

tree::search::Entry::Entry(void)
:
        value_(0),
        rest_(0)
{
}

tree::search::Entry::Entry(int v, Bound t, size_t d, size_t m)
:
        value_(static_cast<int16_t>(v)),
        rest_(0)
{
        rest_ ^= (t & BOUND_MASK) << BOUND_SHIFT;
        rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
        rest_ ^= (m &  MOVE_MASK) <<  MOVE_SHIFT;

        assert(invariant());
}

int tree::search::Entry::value(void) const
{
        return value_;
}

int tree::search::Entry::refined_value(int score) const
{
        return is_cutoff(score)? value() : score;
}

tree::search::Entry::Bound tree::search::Entry::bound(void) const
{
        return static_cast<Bound>((rest_ & (BOUND_MASK << BOUND_SHIFT)) >> BOUND_SHIFT);
}

size_t tree::search::Entry::depth(void) const
{
        return (rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT;
}

size_t tree::search::Entry::move(void) const
{
        return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
}

bool tree::search::Entry::equal_to(size_t d) const
{
        return depth() == d; 
}

bool tree::search::Entry::greater_equal(size_t d) const
{
        return depth() >= d; 
}

bool tree::search::Entry::is_cutoff(int alpha, int beta) const
{
        return is_fail_low(alpha) || is_fail_high(beta);
}

bool tree::search::Entry::is_cutoff(int beta) const
{
        return is_cutoff(beta - 1, beta);
}

bool tree::search::Entry::is_fail_low(int alpha) const
{
        return has_upper() && value() <= alpha;
}

bool tree::search::Entry::is_fail_high(int beta) const
{
        return has_lower() && value() >= beta;
}

bool tree::search::Entry::is_finite(void) const
{
        return score::is_finite(value());
}

bool tree::search::Entry::is_mate(void) const
{
        return score::is_mate(value());
}
bool tree::search::Entry::has_move(void) const
{
        return move() != no_move();
}

bool tree::search::Entry::has_lower(void) const
{
        return (bound() & lower()) != 0;
}

bool tree::search::Entry::has_upper(void) const
{
        return (bound() & upper()) != 0;
}

bool tree::search::Entry::is_lower(void) const
{
        return bound() == lower();
}

bool tree::search::Entry::is_upper(void) const
{
        return bound() == upper();
}

bool tree::search::Entry::is_exact(void) const
{
        return bound() == exact();
}

bool tree::search::Entry::invariant(void) const
{
        return is_finite();             
}

std::string tree::search::Entry::print_value(void) const
{
        std::stringstream sstr(score::print(value()));
        return sstr.str();
}

std::string tree::search::Entry::print_bound(void) const
{
        std::stringstream sstr;
        switch(bound()) {
        case LOWER:
                sstr << ">=";
                break;
        case UPPER:
                sstr << "<=";
                break;
        case EXACT:
                sstr << "==";
                break;
        default:
                assert(false);
        }
        return sstr.str();
}

std::string tree::search::Entry::print_depth(void) const
{
        std::stringstream sstr;
        sstr << depth();
        return sstr.str();
}

std::string tree::search::Entry::print_move(void) const
{
        std::stringstream sstr;
        if (has_move())
                sstr << move();
        else
                sstr << "...";
        return sstr.str();
}

void tree::search::Entry::set_move(size_t m)
{
        rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
        rest_ ^= (m      & MOVE_MASK) << MOVE_SHIFT;
}

tree::search::Entry::Bound tree::search::Entry::lower()
{
        return LOWER;
}

tree::search::Entry::Bound tree::search::Entry::upper()
{
        return UPPER;
}

tree::search::Entry::Bound tree::search::Entry::exact()
{
        return EXACT;
}

size_t tree::search::Entry::no_move()
{
        return MOVE_MASK;
}
