#pragma once
#include <string>

namespace dctl {
namespace dxp {

class MessageInterface
{
public:
        // virtual destructor
        virtual ~MessageInterface() {};

        // non-virtual interface
        std::string str() const;

private:
        // pure virtual implementation
        virtual std::string header() const = 0;
        virtual std::string body() const = 0;
};

}       // namespace dxp
}       // namespace dctl
