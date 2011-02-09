#pragma once
#include "Parser.h"
#include <string>

namespace DamExchangeProtocol {

class AbstractMessage
{
public:
        // interface
        std::string str(void) const;

        // virtual destructor
        virtual ~AbstractMessage(void) {};

private:
        virtual std::string header(void) const = 0;
        virtual std::string body(void) const = 0;
};

}       // namespace DamExchangeProtocol
