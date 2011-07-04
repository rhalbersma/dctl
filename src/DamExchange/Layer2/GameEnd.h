#pragma once
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class GameEnd: public MessageInterface
{
public:
        // typedefs
        enum Reason { FORFEIT = 0, RESIGN = 1, CLAIM_DRAW = 2, CLAIM_WIN = 3 }; 
        enum StopCode { STOP_THIS = 0, STOP_ALL = 1 };

        // views
        Reason reason(void) const;
        StopCode stop_code(void) const;
        static std::string str(Reason, StopCode);

private:
        // private constructor to enforce factory creation
        explicit GameEnd(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(Reason, StopCode);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        Reason reason_;
        StopCode stop_code_;
};

}       // namespace layer2
}       // namespace damexchange
