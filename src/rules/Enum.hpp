#pragma once

namespace dctl {
namespace rules {

// king range
namespace range {
        struct distance_1 {};
        struct distance_N {};
        struct distance_1K {};
}        

// capture Compasss
namespace Compasss {
        struct up {};
        struct down {};
        struct diag {};
        struct orth {};
        struct all {};
}

// capture removal
namespace removal {
        struct apres_fini {};   // having finished
        struct en_passant {};   // in passing
}

// pawn promotion
namespace promotion {
        struct apres_fini {};   // having finished
        struct en_passant {};   // in passing
}

// majority capture precedence
namespace precedence {
        struct none {};
        struct quantity {};
        struct quality {};
}

}       // namespace rules
}       // namespace dctl
