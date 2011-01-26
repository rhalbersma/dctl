#pragma once
#include "AbstractMessage.h"
#include "StringMessage.h"
#include <string>
#include <memory>

namespace DamExchangeProtocol {

class BackRequest: public AbstractMessage
{
public:
        // constructors
        explicit BackRequest(const std::string&);
        BackRequest(size_t, bool);

        // views
        size_t move_number(void) const;
        bool side_to_move(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const StringMessage&);
        static bool pre_condition(const StringMessage&);

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t move_number_;
        bool side_to_move_;
};

}       // namespace DamExchangeProtocol
