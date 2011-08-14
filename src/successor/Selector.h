#pragma once
#include <boost/utility.hpp>    // boost::noncopyable

namespace dctl {
namespace successor {

template<typename, typename> class StateInterface;

template<typename Rules, typename Board> 
class Selector
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef const StateInterface<Rules, Board>* StatePointer;
        static StatePointer find(int);

private:
        // implementation
        typedef const StatePointer StateArray[8];
        static StateArray& instance();
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "Selector.hpp"
