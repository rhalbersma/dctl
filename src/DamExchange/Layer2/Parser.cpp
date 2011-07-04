#include "Parser.h"
#include "../layer1/Scanner.h"

namespace DXP = damexchange;

std::unique_ptr<DXP::layer2::MessageInterface> DXP::layer2::Parser::find(const std::string& msg)
{
        DXP::layer1::Scanner scanner(msg);
        auto i = instance().find(scanner.header());
        return (i != instance().end())? (i->second)(scanner.body()) : std::unique_ptr<MessageInterface>();
}

bool DXP::layer2::Parser::insert(const std::string& header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool DXP::layer2::Parser::erase(const std::string& header)
{
        return instance().erase(header) == 1;
}

DXP::layer2::Parser::CreatorMap& DXP::layer2::Parser::instance(void)
{
        // Meyers Singleton, see Modern C++ Design p.117
        static CreatorMap singleton_;
        return singleton_;
}
