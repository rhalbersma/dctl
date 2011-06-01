#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include "AbstractMessage.h"

namespace DamExchange {
namespace Layer2 {

class GameRequest: public AbstractMessage
{
public:
        // constructors
        explicit GameRequest(const std::string&);
        GameRequest(const std::string&, char, size_t, size_t, bool, const std::string&);

        // views
        const std::string& name_initiator(void) const;
        char color_follower(void) const;
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
        static const char INITIAL;
        static const char SPECIAL;
        static const char SETUP[2];

        static bool read_setup(char);
        static char write_setup(bool);

        // representation
        std::string name_initiator_;
        char color_follower_;
        size_t minutes_;
        size_t moves_;
        bool setup_position_;
        std::string special_position_;
};

}       // namespace Layer2
}       // namespace DamExchange
