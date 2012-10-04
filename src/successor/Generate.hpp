#pragma once
#include <functional>                   // function
#include <boost/assert.hpp>             // BOOST_ASSERT
#include <boost/utility.hpp>            // noncopyable
#include "Invariant.hpp"
#include "Select.hpp"
#include "generation/Generator.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../node/Stack.hpp"

namespace dctl {
namespace successor {

template<typename Position>
Stack generate(Position const& p)
{
        return generate<Material::both, DefaultSelection>(p);
}

template<int Material, typename Selection, typename Position>
Stack generate(Position const& p)
{
        Stack moves;

        // parentheses around function objects to avoid "C++'s most vexing parse"
        (aux::generator<Material, Selection, Position>(moves))(p);
        
        return moves;
}

namespace aux {

// partial specialization for legal successors
template<int Material, typename Selection, typename Position>
struct generator
:
        // enforce reference semantics
        private boost::noncopyable,
        public std::function<void(Position const&)>
{
private:
        // typedefs

        typedef successor::detail::generator<Side::white, Material, Selection, Position> DoWhite;
        typedef successor::detail::generator<Side::black, Material, Selection, Position> DoBlack;

        // representation

        Stack& moves_;

public:
        // structors

        explicit generator(Stack& m)
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

                BOOST_ASSERT((successor::detail::invariant<Material, Selection>(p, moves_.size())));
        }
};

}       // namespace aux
}       // namespace successor
}       // namespace dctl
