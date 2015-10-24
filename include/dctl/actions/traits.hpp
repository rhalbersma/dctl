#pragma once
#include <xstd/pp/tti.hpp>      // XSTD_PP_TTI_CONSTANT

namespace dctl {
namespace actions {

struct generate_tag {};
struct count_tag {};
struct detect_tag {};

XSTD_PP_TTI_CONSTANT(is_jump_precedence, true)
XSTD_PP_TTI_CONSTANT(is_jump_duplicates, false)
XSTD_PP_TTI_CONSTANT(is_jump_path_trace, false)

}       // namespace actions
}       // namespace dctl
