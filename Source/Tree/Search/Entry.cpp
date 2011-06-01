#include <cassert>
#include <sstream>
#include "Entry.h"
#include "Value.h"

Tree::Search::Entry::Entry(void)
:
        value_(0),
        rest_(0)
{
}

Tree::Search::Entry::Entry(int v, Bound t, size_t d, size_t m)
:
        value_(static_cast<int16_t>(v)),
        rest_(0)
{
        rest_ ^= (t & BOUND_MASK) << BOUND_SHIFT;
        rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
        rest_ ^= (m & MOVE_MASK) << MOVE_SHIFT;

        assert(invariant());
}

int Tree::Search::Entry::value(void) const
{
        return value_;
}

int Tree::Search::Entry::refined_value(int score) const
{
        return is_cutoff(score)? value() : score;
}

Tree::Search::Entry::Bound Tree::Search::Entry::bound(void) const
{
        return static_cast<Bound>((rest_ & (BOUND_MASK << BOUND_SHIFT)) >> BOUND_SHIFT);
}

size_t Tree::Search::Entry::depth(void) const
{
        return (rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT;
}

size_t Tree::Search::Entry::move(void) const
{
        return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
}

bool Tree::Search::Entry::equal_to(size_t d) const
{
        return depth() == d; 
}

bool Tree::Search::Entry::greater_equal(size_t d) const
{
        return depth() >= d; 
}

bool Tree::Search::Entry::is_cutoff(int alpha, int beta) const
{
        return is_fail_low(alpha) || is_fail_high(beta);
}

bool Tree::Search::Entry::is_cutoff(int beta) const
{
        return is_cutoff(beta - 1, beta);
}

bool Tree::Search::Entry::is_fail_low(int alpha) const
{
        return has_upper() && value() <= alpha;
}

bool Tree::Search::Entry::is_fail_high(int beta) const
{
        return has_lower() && value() >= beta;
}

bool Tree::Search::Entry::is_finite(void) const
{
        return Value::is_finite(value());
}

bool Tree::Search::Entry::is_mate(void) const
{
        return Value::is_mate(value());
}
bool Tree::Search::Entry::has_move(void) const
{
        return move() != no_move();
}

bool Tree::Search::Entry::has_lower(void) const
{
        return (bound() & lower()) != 0;
}

bool Tree::Search::Entry::has_upper(void) const
{
        return (bound() & upper()) != 0;
}

bool Tree::Search::Entry::is_lower(void) const
{
        return bound() == lower();
}

bool Tree::Search::Entry::is_upper(void) const
{
        return bound() == upper();
}

bool Tree::Search::Entry::is_exact(void) const
{
        return bound() == exact();
}

bool Tree::Search::Entry::invariant(void) const
{
        return is_finite();             
}

std::string Tree::Search::Entry::print_value(void) const
{
        std::stringstream sstr(Value::print(value()));
        return sstr.str();
}

std::string Tree::Search::Entry::print_bound(void) const
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

std::string Tree::Search::Entry::print_depth(void) const
{
        std::stringstream sstr;
        sstr << depth();
        return sstr.str();
}

std::string Tree::Search::Entry::print_move(void) const
{
        std::stringstream sstr;
        if (has_move())
                sstr << move();
        else
                sstr << "...";
        return sstr.str();
}

void Tree::Search::Entry::set_move(size_t m)
{
        rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
        rest_ ^= (m      & MOVE_MASK) << MOVE_SHIFT;
}

Tree::Search::Entry::Bound Tree::Search::Entry::lower()
{
        return LOWER;
}

Tree::Search::Entry::Bound Tree::Search::Entry::upper()
{
        return UPPER;
}

Tree::Search::Entry::Bound Tree::Search::Entry::exact()
{
        return EXACT;
}

size_t Tree::Search::Entry::no_move()
{
        return MOVE_MASK;
}
