#pragma once
#include <boost/mpl/identity.hpp>

namespace dctl {
namespace rules {

// king range
namespace range {
        struct distance_1: boost::mpl::identity<distance_1> {};
        struct distance_N: boost::mpl::identity<distance_N> {};
        struct distance_1K: boost::mpl::identity<distance_1K> {};
}

// capture directions
namespace directions {
        struct up: boost::mpl::identity<up>  {};
        struct down: boost::mpl::identity<down>  {};
        struct diag: boost::mpl::identity<diag>  {};
        struct orth: boost::mpl::identity<orth>  {};
        struct all: boost::mpl::identity<all>  {};
}

// capture removal and pawn promotion
namespace phase {
        struct apres_fini: boost::mpl::identity<apres_fini> {};   // having finished
        struct en_passant: boost::mpl::identity<en_passant> {};   // in passing
}

// king move directions
namespace orthogonality {
        struct none: boost::mpl::identity<none> {};
        struct relative: boost::mpl::identity<relative> {};
        struct absolute: boost::mpl::identity<absolute> {};
}

// majority capture precedence
namespace precedence {
        struct none: boost::mpl::identity<none>  {};
        struct quantity: boost::mpl::identity<quantity>  {};
        struct quality: boost::mpl::identity<quality>  {};
}

}       // namespace rules
}       // namespace dctl
