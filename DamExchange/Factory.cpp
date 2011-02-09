#include "Factory.h"
#include "Parser.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;

std::shared_ptr<DXP::AbstractMessage> DXP::Factory::select_creator(const std::string& s)
{
        Parser p(s);
        CreatorMap::const_iterator i = instance().find(p.header());
        return (i != instance().end())? (i->second)(p.body()) : std::shared_ptr<AbstractMessage>();
}

bool DXP::Factory::register_creator(MessageId header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP::Factory::unregister_creator(MessageId header)
{
        return instance().erase(header) == 1;
}

DXP::Factory::CreatorMap& DXP::Factory::instance(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
