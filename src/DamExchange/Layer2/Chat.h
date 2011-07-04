#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class Chat: public MessageInterface
{
public:
        // views
        std::string message(void) const;
        static std::string str(const std::string&);

private:
        // private constructor to enforce factory creation
        explicit Chat(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(const std::string&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string message_;
};

}       // namespace layer2
}       // namespace damexchange
