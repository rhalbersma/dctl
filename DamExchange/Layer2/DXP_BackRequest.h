#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_BackRequest: public DXP_AbstractMessage
{
public:
        // constructors
        DXP_BackRequest(const std::string&);
        DXP_BackRequest(size_t, bool);

        // views
        char header(void) const;
        std::string message(void) const;
        size_t move_number(void) const;
        bool side_to_move(void) const;

        bool read_color(char) const;
        char write_color(void) const;

        // implementation
        static const char HEADER = 'B';

private:
        // representation
        size_t d_move_number;
        bool d_side_to_move;
};
