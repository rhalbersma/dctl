#pragma once
#include <boost/utility.hpp>                    // noncopyable

namespace dctl {

/*

        The class nonconstructible declares -but does not define-
        a protected destructor. This prevents derived classes from
        being constructed or deleted.

        The class nonconstructible was inspired by boost::noncopyable,
        which privately declares -but does not define- the copy constructor
        and the assignment operator. This prevents derived classes from
        being copied or assigned to.

*/

struct nonconstructible
:
        // prevent value semantics
        private boost::noncopyable
{
protected:
        // structors

        ~nonconstructible() = delete;
};

}       // namespace dctl
