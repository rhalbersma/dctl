#pragma once

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(push)
#pragma warning(disable: 4100)  // unreferenced formal parameter
#endif
/*---------------------------------------------------------------------------*/

// A VC++ compiler bug causes it to believe that p->~T() doesn't reference p.
// http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx
#include "unguarded/stack_alloc.h"

/*---------------------------------------------------------------------------*/
#if (_MSC_VER <= 1600)
#pragma warning(pop)
#endif
/*---------------------------------------------------------------------------*/
