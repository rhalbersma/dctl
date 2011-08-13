#pragma once
#include "../utils/IntegerTypes.h"

namespace dctl {
namespace capture {

class ValueInterface
{
public:
        // predicates
        bool is_large(BitBoard) const;
        bool is_promotion() const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        void toggle_with_king();
        void toggle_promotion();

protected:
        // destructor
        ~ValueInterface() {};

private:
        // (pure) virtual implementation
        virtual bool do_is_large(BitBoard) const = 0;
        virtual bool do_is_promotion() const { return false; };
        virtual void do_increment(BitBoard, BitBoard) {};
        virtual void do_decrement(BitBoard, BitBoard) {};
        virtual void do_toggle_with_king() {};
        virtual void do_toggle_promotion() {};
};

}       // namespace capture
}       // namespace dctl
