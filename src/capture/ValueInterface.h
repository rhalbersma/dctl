#pragma once
#include "../utils/IntegerTypes.h"
#include "../bit/Bit.h"
#include "../rules/Rules.h"

namespace dctl {
namespace capture {

template<typename Rules>
class ValueInterface
{
public:
        // virtual destructor
        ~ValueInterface() {};

        // predicates
        bool is_large(BitBoard) const;
        bool is_promotion() const;

        // modifiers
        void increment(BitBoard, BitBoard);
        void decrement(BitBoard, BitBoard);
        void toggle_with_king();
        void toggle_promotion();

private:
        // as an EXCEPTION, the first two functions below are defined in-class
        // this avoids the following spurious W4 warning on MSVC++ Express 2010:
        // C4514 unreferenced inline function has been removed

        // virtual implementation
        virtual bool do_is_large(BitBoard captured_pieces) const 
        { 
                return bit::count(captured_pieces) >= rules::large_capture<Rules>::value; 
        }

        virtual bool do_is_promotion() const 
        { 
                return false; 
        }

        virtual void do_increment(BitBoard, BitBoard) {}
        virtual void do_decrement(BitBoard, BitBoard) {}
        virtual void do_toggle_with_king() {}
        virtual void do_toggle_promotion() {}
};

}       // namespace capture
}       // namespace dctl

// include template definitions inside header because "export" keyword is not supported by most C++ compilers
#include "ValueInterface.hpp"
