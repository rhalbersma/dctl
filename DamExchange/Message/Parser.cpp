#include "Parser.h"
#include "../Connection/Scanner.h"

namespace DXP = DamExchangeProtocol;
namespace Layer1 = DXP::Connection;
namespace Layer2 = DXP::Message;

std::shared_ptr<Layer2::AbstractMessage> Layer2::Parser::find(const std::string& s)
{
        Layer1::Scanner scanner(s);
        CreatorMap::const_iterator i = instance().find(scanner.header());
        return (i != instance().end())? (i->second)(scanner.body()) : std::shared_ptr<AbstractMessage>();
}

bool Layer2::Parser::insert(const std::string& header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool Layer2::Parser::erase(const std::string& header)
{
        return instance().erase(header) == 1;
}

Layer2::Parser::CreatorMap& Layer2::Parser::instance(void)
{
        static CreatorMap creator_map_;
        return creator_map_;
}
