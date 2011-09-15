#pragma once
#include <map>                  // std::map
#include <memory>               // std::unique_ptr
#include <string>               // std::string
#include <boost/utility.hpp>    // boost::noncopyable            
#include "MessageInterface.hpp"
#include "Protocol.hpp"
#include "Scanner.hpp"

namespace dctl {
namespace dxp {

/*

        The Parser class is the <ConcreteCreator> in a <Factory Method>
        design pattern, with the MessageInterface class as the <Product>, 
        and classes derived from that as instances of a <ConcreteProduct>. 

        All derived message classes MUST register themselves with the factory.

*/

template
<
        typename Protocol = protocol_v1,
        template<typename> class Interface = MessageInterface
>
class Factory
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef Interface<Protocol> InterfaceVersion;
        typedef Scanner<Protocol> ScannerVersion;
        typedef std::unique_ptr<InterfaceVersion> (*Creator)(const std::string&);

        static std::unique_ptr<InterfaceVersion> create_message(const std::string& message)
        {
                ScannerVersion scanner(message);
                auto i = instance().find(scanner.header());
                return (i != instance().end())? (i->second)(scanner.body()) : nullptr;
        }

        static bool register_message(const std::string& header, Creator creator)
        {
                return instance().insert(CreatorMap::value_type(header, creator)).second;
        }

        static bool unregister_message(const std::string& header)
        {
                return instance().erase(header) == 1;
        }

private:
        // implementation
        typedef std::map<std::string, Creator> CreatorMap;
        static CreatorMap& instance()
        {
                // Meyers Singleton, see Modern C++ Design p.117
                static CreatorMap singleton_;
                return singleton_;
        }
};

typedef Factory<> Parser;

}       // namespace dxp
}       // namespace dctl
