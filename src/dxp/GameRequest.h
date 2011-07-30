#pragma once
#include <cstddef>
#include <memory>       // std::unique_ptr
#include <string>
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

class GameRequest: public MessageInterface
{
public:
        // views
        const std::string& name_initiator() const;
        char color_follower() const;
        size_t minutes() const;
        size_t moves() const;
        bool setup() const;
        const std::string& position() const;
        static std::string str(const std::string&, char, size_t, size_t, bool, const std::string&);

private:
        // private constructor to enforce factory creation
        explicit GameRequest(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
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

}       // namespace dxp
}       // namespace dctl
