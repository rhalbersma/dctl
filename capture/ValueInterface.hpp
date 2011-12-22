#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {
namespace capture {

class ValueInterface
{
public:
        // predicates
        bool is_large(BitBoard captured_pieces) const
        {
                return do_is_large(captured_pieces);
        }

        bool is_promotion() const
        {
                return do_is_promotion();
        }

        // modifiers
        void increment(BitBoard target_sq, BitBoard king_targets)
        {
                do_increment(target_sq, king_targets);
        }

        void decrement(BitBoard target_sq, BitBoard king_targets)
        {
                do_decrement(target_sq, king_targets);
        }

        void toggle_with_king()
        {
                do_toggle_with_king();
        }

        void toggle_promotion()
        {
                do_toggle_promotion();
        }

protected:
        // non-virtual destructor
        ~ValueInterface() 
        { 
                /* no-op */ 
        }

private:
        // (pure) virtual implementation
        virtual bool do_is_large(BitBoard /* captured_pieces */) const = 0;
        
        virtual bool do_is_promotion() const 
        { 
                return false; 
        }
        
        virtual void do_increment(BitBoard /* target_sq */, BitBoard /* king_targets */) 
        { 
                /* no-op */ 
        }
        
        virtual void do_decrement(BitBoard /* target_sq */, BitBoard /* king_targets */)
        { 
                /* no-op */ 
        }
        
        virtual void do_toggle_with_king() 
        { 
                /* no-op */ 
        }
        
        virtual void do_toggle_promotion() 
        { 
                /* no-op */ 
        }
};

}       // namespace capture
}       // namespace dctl
