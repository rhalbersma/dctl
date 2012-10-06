#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/utility.hpp>            // noncopyable
#include "Generator.hpp"
#include "Invariant.hpp"
#include "../../node/Side.hpp"
#include "../../node/Stack.hpp"

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for legal successors
template<int Material, typename Selection, typename Position>
struct generate
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef generator<Side::white, Material, Selection, Position> DoWhite;
        typedef generator<Side::black, Material, Selection, Position> DoBlack;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generate(Stack& m)
        : 
                moves_(m)
        {
                moves_.reserve(MOVE_RESERVE);        
        }

        // function call operators

        void operator()(Position const& p) const
        {
                // parentheses around function objects to avoid "C++'s most vexing parse"
                if (p.active_color() == Side::white)
                        (DoWhite(moves_))(p);
                else
                        (DoBlack(moves_))(p);

                BOOST_ASSERT((invariant<Material, Selection>(p, moves_.size())));
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
