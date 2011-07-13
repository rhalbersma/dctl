#pragma once
#include <string>

namespace damexchange {

class MessageInterface
{
public:
        // non-virtual interface
        std::string str() const;

        // virtual destructor
        virtual ~MessageInterface() {};

private:
        // pure virtual implementation
        virtual std::string header() const = 0;
        virtual std::string body() const = 0;
};

}       // namespace damexchange
