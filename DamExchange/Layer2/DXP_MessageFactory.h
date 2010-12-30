#pragma once
#include "DXP_String.h"
#include "DXP_AbstractMessage.h"
#include <map>

class DXP_MessageFactory
{
public:
        // typedefs
        typedef char MessageId;
        typedef DXP_AbstractMessage* (*Creator)(const DXP_String&);

        static DXP_AbstractMessage* select_creator(const DXP_String&);
        static bool register_creator(MessageId, Creator);
        static bool unregister_creator(MessageId);

private:
        // implementation
        typedef std::map<MessageId, Creator> CreatorMap;
        static CreatorMap& creator_map(void);
};
