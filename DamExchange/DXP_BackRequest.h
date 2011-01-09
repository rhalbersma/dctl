#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>

class DXP_BackRequest: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        DXP_BackRequest(size_t, bool);

        // views
        virtual DXP_StringMessage message(void) const;
        
        size_t move_number(void) const;
        bool side_to_move(void) const;

private:
        // implementation
        explicit DXP_BackRequest(const std::string&);

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t move_number_;
        bool side_to_move_;
};
