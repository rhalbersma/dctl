#include <cassert>
#include <sstream>
#include "Transposition.h"
#include "Score.h"

namespace dctl {
namespace search {

Transposition::Transposition()
:
        value_(0),
        rest_(0)
{
}

Transposition::Transposition(int v, Bound t, int d, int m)
:
        value_(static_cast<int16_t>(v)),
        rest_(0)
{
        rest_ ^= (t & BOUND_MASK) << BOUND_SHIFT;
        rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
        rest_ ^= (m &  MOVE_MASK) <<  MOVE_SHIFT;

        assert(invariant());
}

int Transposition::value() const
{
        return value_;
}

int Transposition::refined_value(int score) const
{
        return is_cutoff(score)? value() : score;
}

Transposition::Bound Transposition::bound() const
{
        return static_cast<Bound>((rest_ & (BOUND_MASK << BOUND_SHIFT)) >> BOUND_SHIFT);
}

int Transposition::depth() const
{
        return static_cast<int>((rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT);
}

int Transposition::move() const
{
        return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
}

bool Transposition::is_cutoff(int alpha, int beta) const
{
        return is_fail_low(alpha) || is_fail_high(beta);
}

bool Transposition::is_cutoff(int beta) const
{
        return is_cutoff(beta - 1, beta);
}

bool Transposition::is_fail_low(int alpha) const
{
        return has_upper() && value() <= alpha;
}

bool Transposition::is_fail_high(int beta) const
{
        return has_lower() && value() >= beta;
}

bool Transposition::is_finite() const
{
        // delegate to is_finite() from the enclosing namespace scope "search"
        return search::is_finite(value());
}

bool Transposition::has_move() const
{
        return move() != no_move();
}

bool Transposition::has_lower() const
{
        return (bound() & lower()) != 0;
}

bool Transposition::has_upper() const
{
        return (bound() & upper()) != 0;
}

bool Transposition::is_lower() const
{
        return bound() == lower();
}

bool Transposition::is_upper() const
{
        return bound() == upper();
}

bool Transposition::is_exact() const
{
        return bound() == exact();
}

bool Transposition::invariant() const
{
        return is_finite();             
}

std::string Transposition::print_value() const
{
        std::stringstream sstr(print(value()));
        return sstr.str();
}

std::string Transposition::print_bound() const
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
                break;
        }
        return sstr.str();
}

std::string Transposition::print_depth() const
{
        std::stringstream sstr;
        sstr << depth();
        return sstr.str();
}

std::string Transposition::print_move() const
{
        std::stringstream sstr;
        if (has_move())
                sstr << move();
        else
                sstr << "...";
        return sstr.str();
}

void Transposition::set_move(int m)
{
        rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
        rest_ ^= (m      & MOVE_MASK) << MOVE_SHIFT;
}

Transposition::Bound Transposition::lower()
{
        return LOWER;
}

Transposition::Bound Transposition::upper()
{
        return UPPER;
}

Transposition::Bound Transposition::exact()
{
        return EXACT;
}

int Transposition::no_move()
{
        return MOVE_MASK;
}

}       // namespace search
}       // namespace dctl
