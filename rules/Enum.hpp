#pragma once

namespace dctl {
namespace rules {

// king range
enum { scan_1, scan_N };

// king landing range after intermediate captures
enum { land_1, land_N };

// king landing range after the final capture
enum { halt_1, halt_N, halt_1K };

// initial capture directions
enum { dirs_up, dirs_down, dirs_diag, dirs_orth, dirs_all };

// intermediate capture directions
enum { turn_up, turn_down, turn_diag, turn_orth, turn_all };

// capture removal: apres-fini (having finished) or en-passant (in passing)
enum { remove_af, remove_ep };

// promotion condition: apres-fini (having finished) or en-passant (in passing)
enum { promote_af, promote_ep };

// majority capture precedence: none, single, qualified
enum { precede_0, precede_1, precede_Q };

}       // namespace rules
}       // namespace dctl
