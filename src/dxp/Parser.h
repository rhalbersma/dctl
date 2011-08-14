#pragma once
#include <map>                  // std::map
#include <memory>               // std::unique_ptr
#include <string>               // std::string
#include <boost/utility.hpp>    // boost::noncopyable            
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

/*

The Parser class is the <ConcreteCreator> in a <Factory Method Pattern> with 
the MessageInterface class as the <Product> and classes derived from that as
instances of a <ConcreteProduct>. 

All derived message classes MUST register themselves with the factory.

*/

template<typename Protocol>
class Parser
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef std::unique_ptr<MessageInterface> (*Creator)(const std::string&);

        static std::unique_ptr<MessageInterface> create_message(const std::string&);
        static bool register_message(const std::string&, Creator);
        static bool unregister_message(const std::string&);

private:
        // implementation
        typedef std::map<std::string, Creator> CreatorMap;
        static CreatorMap& instance();
};

}       // namespace dxp
}       // namespace dctl

// include template definitions inside header
#include "Parser.hpp"
