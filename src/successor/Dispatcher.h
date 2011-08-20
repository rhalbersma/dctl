#pragma once
#include <boost/utility.hpp>    // boost::noncopyable

namespace dctl {
namespace successor {

template<typename> class StateInterface;

template<typename Selection, typename Rules, typename Board> 
class Dispatcher
: 
        private boost::noncopyable      // enforce singleton semantics
{
public:
        // typedefs
        typedef const StateInterface<Board>* StatePointer;
        static StatePointer select(int);

private:
        // implementation
        typedef const StatePointer StateArray[8];
        static StateArray& instance();
};

}       // namespace successor
}       // namespace dctl

// include template definitions inside header
#include "Dispatcher.hpp"