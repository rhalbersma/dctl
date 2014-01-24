#pragma once
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <vector>                       // vector
#include <dctl/dxp/message.hpp>         // Message
#include <dctl/factory/creatable.hpp>   // make_creatable

namespace dctl {
namespace dxp {

/*

        The format and semantics of Move are defined at:
        http://www.mesander.nl/damexchange/emove.htm

*/

class Move final
:
        // Curiously Recurring Template Pattern (CRTP)
        public factory::make_creatable<Message, Move, 'M'>
{
public:
        // structors

        explicit Move(std::string const& message)
        :
                seconds_{std::stoi(message.substr(0, 4).c_str())},
                from_sq_{std::stoi(message.substr(4, 2).c_str())},
                dest_sq_{std::stoi(message.substr(6, 2).c_str())},
                num_captured_{std::stoi(message.substr(8, 2).c_str())}
        {
                for (auto i = 0; i < num_captured(); ++i) {
                        auto const index = static_cast<std::size_t>(10 + 2 * i);
                        captured_pieces_.push_back(std::stoi(message.substr(index, 2).c_str()));
                }
        }

        // queries

        int seconds() const
        {
                return seconds_;
        }

        int from_sq() const
        {
                return from_sq_;
        }

        int dest_sq() const
        {
                return dest_sq_;
        }

        int num_captured() const
        {
                return num_captured_;
        }

        std::vector<int> const& captured_pieces() const
        {
                return captured_pieces_;
        }

        // output

        static std::string str(int s, int f, int d, int n, std::vector<int> const& c)
        {
                return identifier() + body(s, f, d, n, c);
        }

private:
        // virtual implementation

        std::string do_header() const override
        {
                return identifier();
        }

        std::string do_body() const override
        {
                return body(seconds(), from_sq(), dest_sq(), num_captured(), captured_pieces());
        }

        static std::string body(int s, int f, int d, int n, std::vector<int> const& c)
        {
                std::stringstream sstr;
                sstr << std::setw( 4) << std::setfill('0') << s;
                sstr << std::setw( 2) << std::setfill('0') << f;
                sstr << std::setw( 2) << std::setfill('0') << d;
                sstr << std::setw( 2) << std::setfill('0') << n;
                for (auto&& x : c)
                        sstr << std::setw(2) << std::setfill('0') << x;
                return sstr.str();
        }

        // representation

        int seconds_;
        int from_sq_;
        int dest_sq_;
        int num_captured_;
        std::vector<int> captured_pieces_;
};

}       // namespace dxp
}       // namespace dctl
