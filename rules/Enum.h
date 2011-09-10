#pragma once

namespace dctl {
namespace rules {

// king range
enum { RANGE_1, RANGE_N };

// promotion condition: on the back row or en-passant
enum { PROMOTE_BR, PROMOTE_EP };

// initial capture directions
enum { DIRS_up, DIRS_down, DIRS_DIAG, DIRS_ORTH, DIRS_ALL };

// intermediate capture directions
enum { SCAN_up, SCAN_down, SCAN_SIDE, SCAN_REST, SCAN_ALL };

// king halt after final capture
enum { HALT_1, HALT_N, HALT_K };

// capture removal: one by one or all at once
enum { REMOVE_1, REMOVE_N };

}       // namespace rules
}       // namespace dctl
