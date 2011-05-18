#pragma once
#include <string>

namespace DamExchange {
namespace Layer2 {

class AbstractMessage
{
public:
        // virtual destructor
        virtual ~AbstractMessage(void) {};

        // non-virtual interface
        std::string str(void) const;

private:
        // pure virtual implementation
        virtual std::string header(void) const = 0;
        virtual std::string body(void) const = 0;
};

}       // namespace Layer2
}       // namespace DamExchange
