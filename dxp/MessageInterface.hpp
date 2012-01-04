#pragma once
#include <string>                       // std::string
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/config.hpp>             // BOOST_STATIC_CONSTANT

namespace dctl {
namespace dxp {

/*

        The MessageInterface class is the <Product> in a <Factory Method>
        Design Pattern, with object instances of its derived classes as  
        <ConcreteProduct>, and with the Parser class as the <ConcreteCreator>.

        All derived message classes MUST be registered with a factory.

        The format and semantics of MessageInterface are defined at:
        http://www.mesander.nl/damexchange/edxplg2.htm

*/

class MessageInterface
{
public:
        // virtual destructor
        virtual ~MessageInterface() 
        { 
                /* no-op */ 
        }

        // non-virtual interface
        std::string str() const 
        {
                BOOST_ASSERT(invariant());
                return do_header() + do_body(); 
        }

        static std::string identifier(const std::string& input)
        {
                return input.substr(0, header_length_);
        }

        static std::string parameter(const std::string& input)
        {
                return input.substr(header_length_);
        }

protected:
        bool invariant() const
        {
                return (
                        do_header().length() == header_length_ &&
                        do_body().length() <= max_body_length_
                );
        }

private:
        // pure virtual implementation
        virtual std::string do_header() const = 0;
        virtual std::string do_body() const = 0;

        // representation
        BOOST_STATIC_CONSTANT(auto, header_length_ = 1);
        BOOST_STATIC_CONSTANT(auto, max_body_length_ = 126);
};

}       // namespace dxp
}       // namespace dctl
