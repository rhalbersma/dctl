#pragma once
#include <dctl/util/type_traits.hpp>	// board_t, set_t
#include <tuple>        		// make_tuple
#include <type_traits>			// decay

namespace dctl {
namespace core {
namespace block_adl {

// http://www.friesdammen.nl/dam/pagefr.php?articleID=20
struct frisian
{
        static constexpr auto width                     = 10;
        static constexpr auto height                    = 10;
        static constexpr auto is_inverted               = false;

        static constexpr auto is_backward_pawn_jump     = true;
        static constexpr auto is_long_ranged_king       = true;

        static constexpr auto is_orthogonal_jump        = true;
        static constexpr auto max_same_king_push        = 3;

        static constexpr auto is_quantity_precedence    = true;
        static constexpr auto is_contents_precedence    = true;
        static constexpr auto is_modality_precedence    = true;

        static constexpr auto precedence = [](auto const& a) {		
                constexpr auto max_captured_pieces = set_t<board_t<std::decay_t<decltype(a)>>>::capacity();
                auto const num_captured_kings = a.num_captured_kings();
                return std::make_tuple(
			max_captured_pieces * (a.num_captured_pieces() + num_captured_kings) - num_captured_kings, 
			a.is_with_king()
		);
        };
};

}       // namespace block_adl

using block_adl::frisian;

}       // namespace core
}       // namespace dctl
