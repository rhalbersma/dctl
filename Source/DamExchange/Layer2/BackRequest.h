#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include "AbstractMessage.h"

namespace damexchange {
namespace layer2 {

class BackRequest: public AbstractMessage
{
public:
        // constructors
        explicit BackRequest(const std::string&);
        BackRequest(size_t, char);

        // views
        size_t move_number(void) const;
        char side_to_move(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t move_number_;
        char side_to_move_;
};

}       // namespace layer2
}       // namespace damexchange
