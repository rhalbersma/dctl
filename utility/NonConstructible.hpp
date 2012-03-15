#pragma once

/*

        The class nonconstructible privately declares -but does not define-
        the default constructor and the destructor. This prevents derived
        classes from being constructed or deleted.

        The class nonconstructible was inspired by the boost::noncopyable,
        which privately declares -but does not define- the copy constructor
        and the assignment operator. This prevents derived classes from being
        copied.

*/

class nonconstructible
{
private:
        nonconstructible();
};
