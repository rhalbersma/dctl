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

// capture removal: en-passant (in passing) or apres-fini (having finished)
enum { remove_ep, remove_af };

// promotion condition: en-passant (in passing) or apres-fini (having finished)
enum { promote_ep, promote_af };

}       // namespace rules
}       // namespace dctl
