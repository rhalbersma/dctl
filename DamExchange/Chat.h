#pragma once
#include "AbstractMessage.h"
#include <memory>
#include <string>

namespace DamExchangeProtocol {

class Chat: public AbstractMessage
{
public:
        // constructors
        explicit Chat(const std::string&);

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string message_;
};

}       // namespace DamExchangeProtocol
