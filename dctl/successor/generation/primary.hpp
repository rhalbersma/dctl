#pragma once
#include <boost/utility.hpp>            // noncopyable
#include <dctl/successor/generation/generator_fwd.hpp>
#include <dctl/node/stack.hpp>

namespace dctl {
namespace successor {
namespace detail {

template<bool, int, typename, typename Position>
struct generator
:
        // enforce reference semantics
        private boost::noncopyable
{
private:
        // representation

        Stack& moves_;

public:
        // structors

        /*explicit*/ generator(Stack& m)
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
