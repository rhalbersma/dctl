#pragma once
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

class Chat: public MessageInterface
{
public:
        // views
        std::string message() const;
        static std::string str(const std::string&);

private:
        // private constructor to enforce factory creation
        explicit Chat(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(const std::string&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string message_;
};

}       // namespace dxp
}       // namespace dctl
