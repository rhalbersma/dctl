#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {

class BackAcknowledge: public MessageInterface
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // views
        AcceptanceCode acceptance_code() const;
        static std::string str(AcceptanceCode);

private:
        // private constructor to enforce factory creation
        explicit BackAcknowledge(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(AcceptanceCode);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        AcceptanceCode acceptance_code_;
};

}       // namespace damexchange
