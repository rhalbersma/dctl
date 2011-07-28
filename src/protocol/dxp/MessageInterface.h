#pragma once
#include <string>

namespace dctl {
namespace protocol {
namespace dxp {

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

}       // namespace dxp
}       // namespace protocol
}       // namespace dctl
