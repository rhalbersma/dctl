#pragma once
#include "DXP_AbstractMessage.h"
#include <string>

class DXP_GameRequest: public DXP_AbstractMessage
{
public:
        DXP_GameRequest(const std::string&);
        DXP_GameRequest(const std::string&, bool, size_t, size_t, bool, bool, const std::string&);

        std::string send_message(void) const;

        static const char HEADER = 'R';
        static const size_t PROTOCOL_VERSION = 1;

private:    
        std::string name_initiator;
        bool color_follower;
        size_t minutes_for_game;
        size_t moves_for_game;
        bool initial_position;
        bool side_to_move;
        std::string setup_position;
};
