#pragma once
#include "AbstractMessage.h"
#include "StringMessage.h"
#include <string>
#include <memory>

namespace DamExchangeProtocol {

class GameAcknowledge: public AbstractMessage
{
public:
        // typedefs
        enum AcceptanceCode { ACCEPT = 0, DECLINE_VERSION = 1, DECLINE_THIS = 2, DECLINE_ALL = 3 };

        // constructors
        static std::shared_ptr<AbstractMessage> create(const StringMessage&);
        explicit GameAcknowledge(const std::string&);
        GameAcknowledge(const std::string&, AcceptanceCode);

        // views
        const std::string& name_follower(void) const;
        AcceptanceCode acceptance_code(void) const;

private:    
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        std::string name_follower_;
        AcceptanceCode acceptance_code_;
};

}       // namespace DamExchangeProtocol
