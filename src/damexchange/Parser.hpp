#include "Scanner.h"

namespace damexchange {

template<typename Protocol>
std::unique_ptr<MessageInterface> Parser<Protocol>::find(const std::string& message)
{
        Scanner<Protocol> scanner(message);
        auto i = instance().find(scanner.header());
        return (i != instance().end())? (i->second)(scanner.body()) : nullptr;
}

template<typename Protocol>
bool Parser<Protocol>::insert(const std::string& header, Creator creator)
{
        return instance().insert(CreatorMap::value_type(header, creator)).second;
}

template<typename Protocol>
bool Parser<Protocol>::erase(const std::string& header)
{
        return instance().erase(header) == 1;
}

template<typename Protocol>
typename Parser<Protocol>::CreatorMap& Parser<Protocol>::instance()
{
        // Meyers Singleton, see Modern C++ Design p.117
        static CreatorMap singleton_;
        return singleton_;
}

}       // namespace damexchange
