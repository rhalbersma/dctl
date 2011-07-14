#include <cassert>
#include <sstream>
#include "Entry.h"
#include "Score.h"

namespace tree {
namespace search {

Entry::Entry()
:
        value_(0),
        rest_(0)
{
}

Entry::Entry(int v, Bound t, int d, size_t m)
:
        value_(static_cast<int16_t>(v)),
        rest_(0)
{
        rest_ ^= (t & BOUND_MASK) << BOUND_SHIFT;
        rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
        rest_ ^= (m &  MOVE_MASK) <<  MOVE_SHIFT;

        assert(invariant());
}

int Entry::value() const
{
        return value_;
}

int Entry::refined_value(int score) const
{
        return is_cutoff(score)? value() : score;
}

Entry::Bound Entry::bound() const
{
        return static_cast<Bound>((rest_ & (BOUND_MASK << BOUND_SHIFT)) >> BOUND_SHIFT);
}

int Entry::depth() const
{
        return static_cast<int>((rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}

size_t Entry::move() const
{
        return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
}

bool Entry::is_cutoff(int alpha, int beta) const
{
        return is_fail_low(alpha) || is_fail_high(beta);
}

bool Entry::is_cutoff(int beta) const
{
        return is_cutoff(beta - 1, beta);
}

bool Entry::is_fail_low(int alpha) const
{
        return has_upper() && value() <= alpha;
}

bool Entry::is_fail_high(int beta) const
{
        return has_lower() && value() >= beta;
}

bool Entry::is_finite() const
{
        return score::is_finite(value());
}

bool Entry::has_move() const
{
        return move() != no_move();
}

bool Entry::has_lower() const
{
        return (bound() & lower()) != 0;
}

bool Entry::has_upper() const
{
        return (bound() & upper()) != 0;
}

bool Entry::is_lower() const
{
        return bound() == lower();
}

bool Entry::is_upper() const
{
        return bound() == upper();
}

bool Entry::is_exact() const
{
        return bound() == exact();
}

bool Entry::invariant() const
{
        return is_finite();             
}

std::string Entry::print_value() const
{
        std::stringstream sstr(score::print(value()));
        return sstr.str();
}

std::string Entry::print_bound() const
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

std::string Entry::print_depth() const
{
        std::stringstream sstr;
        sstr << depth();
        return sstr.str();
}

std::string Entry::print_move() const
{
        std::stringstream sstr;
        if (has_move())
                sstr << move();
        else
                sstr << "...";
        return sstr.str();
}

void Entry::set_move(size_t m)
{
        rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
        rest_ ^= (m      & MOVE_MASK) << MOVE_SHIFT;
}

Entry::Bound Entry::lower()
{
        return LOWER;
}

Entry::Bound Entry::upper()
{
        return UPPER;
}

Entry::Bound Entry::exact()
{
        return EXACT;
}

size_t Entry::no_move()
{
        return MOVE_MASK;
}

}       // namespace search
}       // namespace tree
