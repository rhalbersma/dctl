#include "SearchNode.h"
#include "SearchValue.h"
#include <cassert>
#include <sstream>

SearchNode::SearchNode(void)
:
        value_(0),
        rest_(0)
{
}

SearchNode::SearchNode(int s_value, Type s_type, size_t s_depth, size_t s_move)
:
        value_(static_cast<int16_t>(s_value)),
        rest_(0)
{
        rest_ ^= (s_type & TYPE_MASK) << TYPE_SHIFT;
        rest_ ^= (s_depth & DEPTH_MASK) << DEPTH_SHIFT;
        rest_ ^= (s_move & MOVE_MASK) << MOVE_SHIFT;
}

int SearchNode::value(void) const
{
        return value_;
}

int SearchNode::refined_value(int score) const
{
        return is_cutoff(score)? value() : score;
}

SearchNode::Type SearchNode::type(void) const
{
        return static_cast<Type>((rest_ & (TYPE_MASK << TYPE_SHIFT)) >> TYPE_SHIFT);
}

size_t SearchNode::depth(void) const
{
        return (rest_ & (DEPTH_MASK << DEPTH_SHIFT)) >> DEPTH_SHIFT;
}

size_t SearchNode::move(void) const
{
        return (rest_ & (MOVE_MASK << MOVE_SHIFT)) >> MOVE_SHIFT;
}

bool SearchNode::is_depth_greater_equal(size_t s_depth) const
{
        return depth() >= s_depth; 
}

bool SearchNode::is_cutoff(int alpha, int beta) const
{
        return is_fail_low(alpha) || is_fail_high(beta);
}

bool SearchNode::is_cutoff(int beta) const
{
        return is_fail_low(beta - 1) || is_fail_high(beta);
}

bool SearchNode::is_fail_low(int alpha) const
{
        return has_upper() && value() <= alpha;
}

bool SearchNode::is_fail_high(int beta) const
{
        return has_lower() && value() >= beta;
}

bool SearchNode::is_finite(void) const
{
        return SearchValue::is_finite(value());
}

bool SearchNode::is_heuristic(void) const
{
        return SearchValue::is_heuristic(value());
}
bool SearchNode::has_move(void) const
{
        return move() != no_move();
}

bool SearchNode::has_lower(void) const
{
        return (type() & lower()) != 0;
}

bool SearchNode::has_upper(void) const
{
        return (type() & upper()) != 0;
}

bool SearchNode::is_lower(void) const
{
        return type() == lower();
}

bool SearchNode::is_upper(void) const
{
        return type() == upper();
}

bool SearchNode::is_exact(void) const
{
        return type() == exact();
}

bool SearchNode::invariant(void) const
{
        return is_finite();             
}

std::string SearchNode::print_value(void) const
{
        std::stringstream sstr(SearchValue::print(value()));
        return sstr.str();
}

std::string SearchNode::print_type(void) const
{
        std::stringstream sstr;
        switch(type()) {
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

std::string SearchNode::print_depth(void) const
{
        std::stringstream sstr;
        sstr << depth();
        return sstr.str();
}

std::string SearchNode::print_move(void) const
{
        std::stringstream sstr;
        if (has_move())
                sstr << move();
        else
                sstr << "...";
        return sstr.str();
}

void SearchNode::set_move(size_t s_move)
{
        rest_ ^= (move() & MOVE_MASK) << MOVE_SHIFT;
        rest_ ^= (s_move & MOVE_MASK) << MOVE_SHIFT;
}

SearchNode::Type SearchNode::lower()
{
        return LOWER;
}

SearchNode::Type SearchNode::upper()
{
        return UPPER;
}

SearchNode::Type SearchNode::exact()
{
        return EXACT;
}

size_t SearchNode::no_move()
{
        return MOVE_MASK;
}
