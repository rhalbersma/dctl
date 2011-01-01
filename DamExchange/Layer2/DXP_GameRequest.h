#pragma once
#include "DXP_AbstractMessage.h"
#include "DXP_StringMessage.h"
#include <string>

class DXP_GameRequest: public DXP_AbstractMessage
{
public:
        // constructors
        static DXP_AbstractMessage* create(const DXP_StringMessage&);

        explicit DXP_GameRequest(const std::string&);
        DXP_GameRequest(const std::string&, bool, size_t, size_t, bool, const std::string&);

        // views
        virtual DXP_StringMessage message(void) const;

        const std::string& name_initiator(void) const;
        bool color_follower(void) const;
        size_t minutes(void) const;
        size_t moves(void) const;
        bool setup_position(void) const;
        const std::string& special_position(void) const;

private:
        // implementation
        static const std::string HEADER;
        static const size_t PROTOCOL_VERSION = 1;
        static const bool REGISTERED;

        // representation
        std::string name_initiator_;
        bool color_follower_;
        size_t minutes_;
        size_t moves_;
        bool setup_position_;
        std::string special_position_;
};
