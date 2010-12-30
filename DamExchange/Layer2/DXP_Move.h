#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>
#include <vector>

class DXP_Move: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_Move(const std::string&);
        DXP_Move(size_t, size_t, size_t, size_t, const std::vector<size_t>&);

        // views
        virtual DXP_String message(void) const;        

        size_t seconds(void) const;
        size_t from_sq(void) const;
        size_t dest_sq(void) const;
        size_t num_captured(void) const;
        const std::vector<size_t>& captured_pieces(void) const;

private:
        // implementation
        static const char HEADER = 'M';
        static const bool REGISTERED;

        // representation
        size_t d_seconds;
        size_t d_from_sq;
        size_t d_dest_sq;
        size_t d_num_captured;
        std::vector<size_t> d_captured_pieces;       
};
