#pragma once
#include "StringMessage.h"
#include "AbstractMessage.h"
#include <map>
#include <memory>

namespace DamExchangeProtocol {

class MessageFactory
{
public:
        // typedefs
        typedef std::string MessageId;
        typedef std::shared_ptr<AbstractMessage> (*Creator)(const StringMessage&);

        static std::shared_ptr<AbstractMessage> select_creator(const StringMessage&);
        static bool register_creator(MessageId, Creator);
        static bool unregister_creator(MessageId);

private:
        // implementation
        typedef std::map<MessageId, Creator> CreatorMap;
        static CreatorMap& instance(void);
};

}       // namespace DamExchangeProtocol
