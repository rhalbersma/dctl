#pragma once
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

/*

The GameRequest class is a <ConcreteProduct> in a <Factory Method Pattern>
with the Parser class as the <ConcreteCreator> and the MessageInterface class 
as the <Product>.

The GameRequest class registers itself with the factory.

*/

class GameRequest
: 
        public MessageInterface
{
public:
        // views
        const std::string& name_initiator() const;
        char color_follower() const;
        int minutes() const;
        int moves() const;
        bool setup() const;
        const std::string& position() const;
        static std::string str(const std::string&, char, int, int, bool, const std::string&);

private:
        // private constructor to enforce factory creation
        explicit GameRequest(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(const std::string&, char, int, int, bool, const std::string&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const int PROTOCOL_VERSION = 1;
        static const bool REGISTERED;
        static const char INITIAL;
        static const char SPECIAL;
        static const char SETUP[2];

        static bool read_setup(char);
        static char write_setup(bool);

        // representation
        std::string name_initiator_;
        char color_follower_;
        int minutes_;
        int moves_;
        bool setup_;
        std::string position_;
};

}       // namespace dxp
}       // namespace dctl
