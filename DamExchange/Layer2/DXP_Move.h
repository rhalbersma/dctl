#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>
#include <vector>

class DXP_Move: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        explicit DXP_Move(const std::string&);
        DXP_Move(size_t, size_t, size_t, size_t, const std::vector<size_t>&);

        // views
        virtual DXP_StringMessage message(void) const;        

        size_t seconds(void) const;
        size_t from_sq(void) const;
        size_t dest_sq(void) const;
        size_t num_captured(void) const;
        const std::vector<size_t>& captured_pieces(void) const;

private:
        // implementation
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t seconds_;
        size_t from_sq_;
        size_t dest_sq_;
        size_t num_captured_;
        std::vector<size_t> captured_pieces_;       
};
