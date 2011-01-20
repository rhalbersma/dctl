#include "MessageFactory.h"
#include <cassert>

namespace DXP  = DamExchangeProtocol;

std::shared_ptr<DXP::AbstractMessage> DXP::MessageFactory::select_creator(const DXP::StringMessage& s)
{
        CreatorMap::const_iterator i = creator_map().find(s.header());
        if (i != creator_map().end())
                return (i->second)(s);
        
        assert(false);
        return std::shared_ptr<DXP::AbstractMessage>();  // nullptr
}

bool DXP::MessageFactory::register_creator(MessageId header, Creator creator)
{
        return creator_map().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP::MessageFactory::unregister_creator(MessageId header)
{
        return creator_map().erase(header) == 1;
}

DXP::MessageFactory::CreatorMap& DXP::MessageFactory::creator_map(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
