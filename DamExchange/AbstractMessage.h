#pragma once
#include "StringMessage.h"

namespace DamExchangeProtocol {

class AbstractMessage
{
public:
        // interface
        StringMessage message(void) const;

        // virtual destructor
        virtual ~AbstractMessage(void) {};

private:
        virtual std::string header(void) const = 0;
        virtual std::string body(void) const = 0;
};

}       // namespace DamExchangeProtocol
