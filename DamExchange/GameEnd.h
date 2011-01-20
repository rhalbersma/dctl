#pragma once
#include "AbstractMessage.h"
#include "StringMessage.h"
#include <string>
#include <memory>

namespace DamExchangeProtocol {

class GameEnd: public AbstractMessage
{
public:
        // typedefs
        enum Reason { FORFEIT = 0, RESIGN = 1, CLAIM_DRAW = 2, CLAIM_WIN = 3 }; 
        enum StopCode { STOP_THIS = 0, STOP_ALL = 1 };

        // constructors
        static std::shared_ptr<AbstractMessage> create(const StringMessage&);
        explicit GameEnd(const std::string&);
        GameEnd(Reason, StopCode);

        // views
        Reason reason(void) const;
        StopCode stop_code(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        Reason reason_;
        StopCode stop_code_;
};

}       // namespace DamExchangeProtocol
