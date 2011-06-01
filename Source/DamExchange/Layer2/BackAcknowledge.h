#pragma once
#include <memory>
#include <string>
#include "AbstractMessage.h"

namespace DamExchange {
namespace Layer2 {

class BackAcknowledge: public AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, NOT_SUPPORTED = 1, DECLINE = 2 };

        // constructors
        explicit BackAcknowledge(const std::string&);
        explicit BackAcknowledge(AcceptanceCode);

        // views
        AcceptanceCode acceptance_code(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        AcceptanceCode acceptance_code_;
};

}       // namespace Layer2
}       // namespace DamExchange
