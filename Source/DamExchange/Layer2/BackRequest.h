#pragma once
#include <cstddef>
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class BackRequest: public MessageInterface
{
public:
        // views
        size_t move_number(void) const;
        char side_to_move(void) const;
        static std::string str(size_t, char);

private:
        // private constructor to enforce factory creation
        explicit BackRequest(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(size_t, char);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t move_number_;
        char side_to_move_;
};

}       // namespace layer2
}       // namespace damexchange
