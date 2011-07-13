#pragma once
#include <cstddef>
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {

class BackRequest: public MessageInterface
{
public:
        // views
        size_t move_number() const;
        char side_to_move() const;
        static std::string str(size_t, char);

private:
        // private constructor to enforce factory creation
        explicit BackRequest(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(size_t, char);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t move_number_;
        char side_to_move_;
};

}       // namespace damexchange
