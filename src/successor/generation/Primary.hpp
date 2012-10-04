#pragma once
#include <functional>                   // function
#include <boost/utility.hpp>            // noncopyable
#include "Generator_fwd.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct generator
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&, Stack&)>
{
private:
        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
        : 
                moves_(m)         
        {}

        // function call operators

        void operator()(Position const& p) const
        {
                // no-op
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
