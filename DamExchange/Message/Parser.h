#pragma once
#include "AbstractMessage.h"
#include <map>
#include <memory>
#include <string>

namespace DamExchangeProtocol {
namespace Message {

class Parser
{
public:
        // typedefs
        typedef std::shared_ptr<AbstractMessage> (*Creator)(const std::string&);

        static std::shared_ptr<AbstractMessage> find(const std::string&);
        static bool insert(const std::string&, Creator);
        static bool erase(const std::string&);

private:
        // implementation
        typedef std::map<std::string, Creator> CreatorMap;
        static CreatorMap& instance(void);
};

}       // namespace Message
}       // namespace DamExchangeProtocol
