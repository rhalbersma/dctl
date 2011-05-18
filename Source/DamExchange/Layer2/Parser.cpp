#include "Parser.h"
#include "../Layer1/Scanner.h"

namespace DXP = DamExchange;

std::shared_ptr<DXP::Layer2::AbstractMessage> DXP::Layer2::Parser::find(const std::string& msg)
{
        DXP::Layer1::Scanner scanner(msg);
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
        // Meyers Singleton, see Modern C++ Design p.117
        static CreatorMap singleton_;
        return singleton_;
}
