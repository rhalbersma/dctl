#pragma once
#include "DXP_AbstractMessage.h"
#include <string>
#include <vector>

class DXP_Move: public DXP_AbstractMessage
{
public:
        // constructors
        DXP_Move(const std::string&);
        DXP_Move(size_t, size_t, size_t, size_t, const std::vector<size_t>&);

        // views
        char header(void) const;
        std::string message(void) const;        
        size_t seconds(void) const;
        size_t from_sq(void) const;
        size_t dest_sq(void) const;
        size_t num_captured(void) const;
        const std::vector<size_t>& captured_pieces(void) const;

        // implementation
        static const char HEADER = 'M';

private:
        // representation
        size_t d_seconds;
        size_t d_from_sq;
        size_t d_dest_sq;
        size_t d_num_captured;
        std::vector<size_t> d_captured_pieces;       
};
