#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

class GameAcknowledge: public MessageInterface
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        // views
        const std::string& name_follower() const;
        AcceptanceCode acceptance_code() const;
        static std::string str(const std::string&, AcceptanceCode);

private:    
        // private constructor to enforce factory creation
        explicit GameAcknowledge(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(const std::string&, AcceptanceCode);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace dxp
}       // namespace dctl
