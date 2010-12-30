#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>

class DXP_BackRequest: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_BackRequest(const std::string&);
        DXP_BackRequest(size_t, bool);

        // views
        virtual DXP_String message(void) const;
        
        size_t move_number(void) const;
        bool side_to_move(void) const;

private:
        // implementation
        static const char HEADER = 'B';
        static const bool REGISTERED;

        // representation
        size_t d_move_number;
        bool d_side_to_move;
};
