#pragma once
#include <algorithm>                    // for_each
#include <iomanip>                      // setfill, setw
#include <sstream>                      // stringstream
#include <string>                       // string
#include <vector>                       // vector
#include <boost/lexical_cast.hpp>       // lexical_cast
#include "MessageInterface.hpp"
#include "../factory/mixin.hpp"

namespace dctl {
namespace dxp {

/*

        The format and semantics of Move are defined at:
        http://www.mesander.nl/damexchange/emove.htm

*/

class Move
:
        // Curiously Recurring Template Pattern (CRTP)
        public mixin::FactoryCreate<'M', Move, MessageInterface>
{
public:
        // structors

        explicit Move(std::string const& message)
        :
                seconds_(boost::lexical_cast<int>(message.substr(0, 4).c_str())),
                from_sq_(boost::lexical_cast<int>(message.substr(4, 2).c_str())),
                dest_sq_(boost::lexical_cast<int>(message.substr(6, 2).c_str())),
                num_captured_(boost::lexical_cast<int>(message.substr(8, 2).c_str()))
        {
                for (auto i = 0; i < num_captured(); ++i)
                        captured_pieces_.push_back(boost::lexical_cast<int>(message.substr(10 + 2 * i, 2).c_str()));
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

        const std::vector<int>& captured_pieces() const
        {
                return captured_pieces_;
        }

        // output

        static std::string str(int s, int f, int d, int n, const std::vector<int>& c)
        {
                return identifier() + body(s, f, d, n, c);
        }

private:
        // virtual implementation

        virtual std::string do_header() const
        {
                return identifier();
        }

        virtual std::string do_body() const
        {
                return body(seconds(), from_sq(), dest_sq(), num_captured(), captured_pieces());
        }

        static std::string body(int s, int f, int d, int n, const std::vector<int>& c)
        {
                std::stringstream sstr;
                sstr << std::setw( 4) << std::setfill('0') << s;
                sstr << std::setw( 2) << std::setfill('0') << f;
                sstr << std::setw( 2) << std::setfill('0') << d;
                sstr << std::setw( 2) << std::setfill('0') << n;
                std::for_each(std::begin(c), std::end(c), [&sstr](int x) {
                        sstr << std::setw(2) << std::setfill('0') << x;
                });
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
