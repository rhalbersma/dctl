#pragma once

namespace dctl {
namespace rules {

// initial king range
enum { range_1, range_N };

// intermediate king range
enum { turn_1, turn_N };

// final king range
enum { halt_1, halt_N, halt_1K };

// initial capture directions
enum { dirs_up, dirs_down, dirs_diag, dirs_orth, dirs_all };

// intermediate capture directions
enum { scan_up, scan_down, scan_diag, scan_orth, scan_all };

// capture removal: en-passant (in passing) or apres-fini (having finished)
enum { remove_ep, remove_af };

// promotion condition: en-passant (in passing) or apres-fini (having finished)
enum { promote_ep, promote_af };

}       // namespace rules
}       // namespace dctl
