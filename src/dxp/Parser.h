#pragma once
#include <map>                  // std::map
#include <memory>               // std::unique_ptr
#include <string>               // std::string
#include <boost/utility.hpp>    // boost::noncopyable            
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

template<typename Protocol>
class Parser
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef std::unique_ptr<MessageInterface> (*Creator)(const std::string&);

        static std::unique_ptr<MessageInterface> find(const std::string&);
        static bool insert(const std::string&, Creator);
        static bool erase(const std::string&);

private:
        // implementation
        typedef std::map<std::string, Creator> CreatorMap;
        static CreatorMap& instance();
};

}       // namespace dxp
}       // namespace dctl

// include template definitions inside header
#include "Parser.hpp"
