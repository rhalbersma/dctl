#pragma once
#include <cstddef>
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace damexchange {
namespace layer2 {

class GameRequest: public MessageInterface
{
public:
        // views
        const std::string& name_initiator(void) const;
        char color_follower(void) const;
        size_t minutes(void) const;
        size_t moves(void) const;
        bool setup(void) const;
        const std::string& position(void) const;
        static std::string str(const std::string&, char, size_t, size_t, bool, const std::string&);

private:
        // private constructor to enforce factory creation
        explicit GameRequest(const std::string&);

        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;
        static std::string body(const std::string&, char, size_t, size_t, bool, const std::string&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
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
        bool setup_;
        std::string position_;
};

}       // namespace layer2
}       // namespace damexchange
