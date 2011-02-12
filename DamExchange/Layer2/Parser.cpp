#include "Parser.h"
#include "../Layer1/Scanner.h"

namespace DXP = DamExchange;

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::Parser::find(const std::string& s)
{
        DXP::Layer1::Scanner scanner(s);
        CreatorMap::const_iterator i = instance().find(scanner.header());
        return (i != instance().end())? (i->second)(scanner.body()) : std::shared_ptr<AbstractMessage>();
}

bool DXP::Layer2::Parser::insert(const std::string& header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP::Layer2::Parser::erase(const std::string& header)
{
        return instance().erase(header) == 1;
}

DXP::Layer2::Parser::CreatorMap& DXP::Layer2::Parser::instance(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
