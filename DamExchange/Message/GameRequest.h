#pragma once
#include "AbstractMessage.h"
#include <memory>
#include <string>

namespace DamExchangeProtocol {
namespace Message {

class GameRequest: public AbstractMessage
{
public:
        // constructors
        explicit GameRequest(const std::string&);
        GameRequest(const std::string&, bool, size_t, size_t, bool, const std::string&);

        // views
        const std::string& name_initiator(void) const;
        bool color_follower(void) const;
        size_t minutes(void) const;
        size_t moves(void) const;
        bool setup_position(void) const;
        const std::string& special_position(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const std::string&);
        static const std::string HEADER;
        static const size_t PROTOCOL_VERSION = 1;
        static const bool REGISTERED;

        // representation
        std::string name_initiator_;
        bool color_follower_;
        size_t minutes_;
        size_t moves_;
        bool setup_position_;
        std::string special_position_;
};

}       // namespace Message
}       // namespace DamExchangeProtocol
