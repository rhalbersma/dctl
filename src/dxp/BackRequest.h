#pragma once
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

class BackRequest: public MessageInterface
{
public:
        // views
        int move_number() const;
        char side_to_move() const;
        static std::string str(int, char);

private:
        // private constructor to enforce factory creation
        explicit BackRequest(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(int, char);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        int move_number_;
        char side_to_move_;
};

}       // namespace dxp
}       // namespace dctl
