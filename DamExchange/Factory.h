#pragma once
#include "AbstractMessage.h"
#include <map>
#include <memory>
#include <string>

namespace DamExchangeProtocol {

class Factory
{
public:
        // typedefs
        typedef std::string MessageId;
        typedef std::shared_ptr<AbstractMessage> (*Creator)(const std::string&);

        static std::shared_ptr<AbstractMessage> select_creator(const std::string&);
        static bool register_creator(MessageId, Creator);
        static bool unregister_creator(MessageId);

private:
        // implementation
        typedef std::map<MessageId, Creator> CreatorMap;
        static CreatorMap& instance(void);
};

}       // namespace DamExchangeProtocol
