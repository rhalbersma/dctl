#pragma once
#include <memory>
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class Chat: public MessageInterface
{
public:
        // constructors
        explicit Chat(const std::string&);

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string message_;
};

}       // namespace layer2
}       // namespace damexchange
