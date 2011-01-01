#include "DXP_MessageFactory.h"
#include <cassert>

DXP_AbstractMessage* DXP_MessageFactory::select_creator(const DXP_StringMessage& s)
{
        CreatorMap::const_iterator i = creator_map().find(s.header());
        if (i != creator_map().end())
                return (i->second)(s);
        
        assert(false);
        return(0);
}

bool DXP_MessageFactory::register_creator(MessageId header, Creator creator)
{
        return creator_map().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP_MessageFactory::unregister_creator(MessageId header)
{
        return creator_map().erase(header) == 1;
}

DXP_MessageFactory::CreatorMap& DXP_MessageFactory::creator_map(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
