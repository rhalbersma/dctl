#pragma once
#include <sstream>                      // std::stringstream
#include <string>                       // std::string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT
#include "Score.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace search {

class Transposition
{
public:
        // typedefs
        enum Bound { lower_bound = 1, upper_bound = 2, exact_value = 3 };

        // constructors
	Transposition()
        :
                value_(0),
                rest_(0)
        {
        }

	Transposition(int v, Bound t, int d, int m)
        :
                value_(static_cast<int16_t>(v)),
                rest_(0)
        {
                rest_ ^= (t & BOUND_MASK) << BOUND_SHIFT;
                rest_ ^= (d & DEPTH_MASK) << DEPTH_SHIFT;
                rest_ ^= (m &  MOVE_MASK) <<  MOVE_SHIFT;

                BOOST_ASSERT(invariant());
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
        Bound bound() const
        {
                return static_cast<Bound>((rest_ & (BOUND_MASK << BOUND_SHIFT)) >> BOUND_SHIFT);
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
        std::string print_value() const
        {
                std::stringstream sstr(print(value()));
                return sstr.str();
        }
        
        std::string print_bound() const
        {
                std::stringstream sstr;
                switch(bound()) {
                case lower_bound:
                        sstr << ">=";
                        break;
                case upper_bound:
                        sstr << "<=";
                        break;
                case exact_value:
                        sstr << "==";
                        break;
                default:
                        BOOST_ASSERT(false);
                        break;
                }
                return sstr.str();
        }
        
        std::string print_depth() const
        {
                std::stringstream sstr;
                sstr << depth();
                return sstr.str();
        }
        
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
        bool is_finite() const
        {
                // delegate to is_finite() from the enclosing namespace scope "search"
                return search::is_finite(value());
        }

        bool has_lower_bound() const
        {
                return (bound() & lower_bound) != 0;
        }
        
        bool has_upper_bound() const
        {
                return (bound() & upper_bound) != 0;
        }
        
        bool is_lower_bound() const
        {
                return bound() == lower_bound;
        }
        
        bool is_upper_bound() const
        {
                return bound() == upper_bound;
        }
        
        bool is_exact_value() const
        {
                return bound() == exact_value;
        }
        
        bool invariant() const
        {
                return is_finite();             
        }

        BOOST_STATIC_CONSTANT(auto, BOUND_BITS = 2);
        BOOST_STATIC_CONSTANT(auto, DEPTH_BITS = 7);
        BOOST_STATIC_CONSTANT(auto, MOVE_BITS = 7);

        BOOST_STATIC_CONSTANT(auto, BOUND_SHIFT = 0);
        BOOST_STATIC_CONSTANT(auto, DEPTH_SHIFT = BOUND_SHIFT + BOUND_BITS);
        BOOST_STATIC_CONSTANT(auto, MOVE_SHIFT = DEPTH_SHIFT + DEPTH_BITS);

        BOOST_STATIC_CONSTANT(auto, BOUND_MASK = ((1 << BOUND_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, DEPTH_MASK = ((1 << DEPTH_BITS) - 1));
        BOOST_STATIC_CONSTANT(auto, MOVE_MASK = ((1 << MOVE_BITS) - 1));

        // representation
         int16_t value_;        // value
        uint16_t rest_;         // bound, depth and move
};

}       // namespace search
}       // namespace dctl
