#pragma once

/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
        #if (_MSC_VER <= 1700)
                #pragma warning(push)
                #pragma warning(disable: 4512)  // assignment operator could not be generated
        #endif
#endif
/*---------------------------------------------------------------------------*/

// http://msdn.microsoft.com/en-us/library/hsyx7kbz(v=vs.100).aspx
// You can resolve the C4512 warning for your code in one of three ways:
// -Explicitly define an assignment operator for the class.
// -Remove const or the reference operator from the data item in the class.
// -Use the #pragma warning statement to suppress the warning.

// Because this is code we cannot touch, we use the third recommendation
#include <boost/algorithm/cxx11/is_permutation.hpp>

/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
        #if (_MSC_VER <= 1700)
                #pragma warning(pop)
        #endif
#endif
/*---------------------------------------------------------------------------*/
