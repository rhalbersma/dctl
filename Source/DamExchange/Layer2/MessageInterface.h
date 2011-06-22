#pragma once
#include <string>

namespace damexchange {
namespace layer2 {

class MessageInterface
{
public:
        // virtual destructor
        virtual ~MessageInterface(void) {};

        // non-virtual interface
        std::string str(void) const;

private:
        // pure virtual implementation
        virtual std::string header(void) const = 0;
        virtual std::string body(void) const = 0;
};

}       // namespace layer2
}       // namespace damexchange
