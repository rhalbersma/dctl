#include "Parser.h"
#include "../layer1/Scanner.h"

namespace damexchange {
namespace layer2 {

std::unique_ptr<MessageInterface> Parser::find(const std::string& msg)
{
        layer1::Scanner scanner(msg);
        auto i = instance().find(scanner.header());
        return (i != instance().end())? (i->second)(scanner.body()) : nullptr;
}

bool Parser::insert(const std::string& header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

bool Parser::erase(const std::string& header)
{
        return instance().erase(header) == 1;
}

Parser::CreatorMap& Parser::instance()
{
        // Meyers Singleton, see Modern C++ Design p.117
        static CreatorMap singleton_;
        return singleton_;
}

}       // namespace layer2
}       // namespace damexchange
