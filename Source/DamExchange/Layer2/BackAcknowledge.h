#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class BackAcknowledge: public MessageInterface
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // views
        AcceptanceCode acceptance_code(void) const;
        static std::string str(AcceptanceCode);

private:
        // private constructor to enforce factory creation
        explicit BackAcknowledge(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(AcceptanceCode);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        AcceptanceCode acceptance_code_;
};


}       // namespace layer2
}       // namespace damexchange
