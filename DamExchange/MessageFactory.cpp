#include "MessageFactory.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;

std::shared_ptr<DXP::AbstractMessage> DXP::MessageFactory::select_creator(const StringMessage& s)
{
        CreatorMap::const_iterator i = instance().find(s.header());
        return (i != instance().end())? (i->second)(s) : std::shared_ptr<AbstractMessage>();
}

bool DXP::MessageFactory::register_creator(MessageId header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP::MessageFactory::unregister_creator(MessageId header)
{
        return instance().erase(header) == 1;
}

DXP::MessageFactory::CreatorMap& DXP::MessageFactory::instance(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
