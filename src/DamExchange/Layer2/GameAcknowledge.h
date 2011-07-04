#pragma once
#include <memory>       // unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class GameAcknowledge: public MessageInterface
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        // views
        const std::string& name_follower(void) const;
        AcceptanceCode acceptance_code(void) const;
        static std::string str(const std::string&, AcceptanceCode);

private:    
        // private constructor to enforce factory creation
        explicit GameAcknowledge(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(const std::string&, AcceptanceCode);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace layer2
}       // namespace damexchange
