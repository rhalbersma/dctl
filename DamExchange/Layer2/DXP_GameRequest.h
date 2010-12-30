#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_String.h"
#include <string>

class DXP_GameRequest: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_String&);

        DXP_GameRequest(const std::string&);
        DXP_GameRequest(const std::string&, bool, size_t, size_t, bool, const std::string&);

        // views
        virtual DXP_String message(void) const;

        const std::string& name_initiator(void) const;
        bool color_follower(void) const;
        size_t minutes(void) const;
        size_t moves(void) const;
        bool setup_position(void) const;
        const std::string& special_position(void) const;

private:
        // implementation
        static const char HEADER = 'R';
        static const size_t PROTOCOL_VERSION = 1;
        static const bool REGISTERED;

        // representation
        std::string d_name_initiator;
        bool d_color_follower;
        size_t d_minutes;
        size_t d_moves;
        bool d_setup_position;
        std::string d_special_position;
};
