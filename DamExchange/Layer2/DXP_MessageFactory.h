#pragma once
#include "DXP_StringMessage.h"
#include "DXP_AbstractMessage.h"
#include <map>

class DXP_MessageFactory
{
public:
        // typedefs
        typedef std::string MessageId;
        typedef DXP_AbstractMessage* (*Creator)(const DXP_StringMessage&);

        static DXP_AbstractMessage* select_creator(const DXP_StringMessage&);
        static bool register_creator(MessageId, Creator);
        static bool unregister_creator(MessageId);

private:
        // implementation
        typedef std::map<MessageId, Creator> CreatorMap;
        static CreatorMap& creator_map(void);
};
