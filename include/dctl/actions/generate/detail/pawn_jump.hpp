#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, ToggleKingTargets, SetPromotion
#include <dctl/actions/detail/builder.hpp>              // Builder
#include <dctl/actions/generate/detail/primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/generate/detail/king_jump.hpp>   // promote_en_passant
#include <dctl/actions/select/jump.hpp>                 // jumps
#include <dctl/board/angle.hpp>                         // _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror, turn
#include <dctl/board/wave.hpp>                          // make_iterator
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/rule_traits.hpp>                         // is_pawn_jump_king_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/state/promotion.hpp>                     // is_promotion
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t, set_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, class Reverse, class Builder, class Sequence>
class Generate<ToMove, Piece::pawn, select::jump, Reverse, Builder, Sequence>
{
        using  KingJumps = Generate<ToMove, Piece::king, select::jump, Reverse, Builder, Sequence>;
        using board_type = board_t<Builder>;
        using rules_type = rules_t<Builder>;
        using   set_type =   set_t<Builder>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        Builder& builder;
        Sequence& moves;

public:
        explicit Generate(Builder& b, Sequence& m)
        :
                builder{b},
                moves{m}
        {}

        auto operator()(set_type const& active_pawns) const
        {
                if (active_pawns.any())
                        pawn_jump_king_dispatch(active_pawns, is_pawn_jump_king_t<rules_type>{});
        }

private:
        auto pawn_jump_king_dispatch(set_type const& active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        auto pawn_jump_king_dispatch(set_type const& active_pawns, std::false_type) const
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                branch(active_pawns);
        }

        auto branch(set_type const& active_pawns) const
        {
                branch_dispatch(active_pawns, pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        auto branch_dispatch(set_type const& active_pawns, forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
        }

        auto branch_dispatch(set_type const& active_pawns, backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left_down (orientation)>(active_pawns);
                serialize<right_down(orientation)>(active_pawns);
        }

        auto branch_dispatch(set_type const& active_pawns, forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                serialize<up        (orientation)>(active_pawns);
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left      (orientation)>(active_pawns);
                serialize<right     (orientation)>(active_pawns);
        }

        auto branch_dispatch(set_type const& active_pawns, backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                serialize<up        (orientation)>(active_pawns);
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left      (orientation)>(active_pawns);
                serialize<right     (orientation)>(active_pawns);
                serialize<left_down (orientation)>(active_pawns);
                serialize<right_down(orientation)>(active_pawns);
                serialize<down      (orientation)>(active_pawns);
        }

        template<int Direction>
        auto serialize(set_type const& active_pawns) const
        {
                auto const jumpers = active_pawns & set_type(*std::prev(along_wave<Direction>(builder.template targets<Direction>())));
                jumpers.for_each([&](auto const& from_sq){
                        find_first(along_ray<Direction>(from_sq));
                });
        }

        template<class Iterator>
        auto find_first(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Launch<Builder> guard{builder, *jumper};
                capture(std::next(jumper));
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Capture<Builder> guard{builder, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                try_promotion(jumper);
        }

        template<class Iterator>
        auto try_promotion(Iterator jumper) const
        {
                try_promotion_dispatch(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, stopped_promotion_tag) const
        {
                if (find_next(jumper))
                        return;
                if (is_promotion(*jumper))
                        return on_promotion(jumper);
                add_jump(*jumper);
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, passing_promotion_tag) const
        {
                if (is_promotion(*jumper))
                        return on_promotion(jumper);
                try_next(jumper);
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::SetPromotion<Builder> guard{builder};
                on_promotion_dispatch(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, stopped_promotion_tag) const
        {
                add_jump(*jumper);
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, passing_promotion_tag) const
        {
                king_jumps(jumper);
        }

        template<class Iterator>
        auto king_jumps(Iterator jumper) const
        {
                king_jumps_dispatch(jumper, is_pawn_jump_king_t<rules_type>{});
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, std::true_type) const
        {
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_dispatch(Iterator jumper, std::false_type) const
        {
                raii::ToggleKingTargets<Builder> guard{builder};
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                KingJumps{builder, moves}.try_next(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!find_next(jumper))
                        add_jump(*jumper);
        }

        template<class Iterator>
        auto find_next(Iterator jumper) const
        {
                //raii::Visit<Builder> guard{builder, *jumper};
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                return turn_dispatch(jumper, pawn_jump_category_t<rules_type>{}, jump_category_t<rules_type>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, forward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>));
                return scan(ray::mirror<up(orientation)>(jumper));
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, diagonal_jump_tag) const
        {
                static_assert(is_diagonal(direction_v<Iterator>));
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, forward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                static_assert(!is_down(direction_v<Iterator>));
                return turn_dispatch(jumper, angle_constant<direction_v<Iterator>>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_constant<up(orientation)>) const
        {
                return
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_constant<left_up(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_constant<right_up(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_constant<left(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_constant<right(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, backward_pawn_jump_tag, orthogonal_jump_tag) const
        {
                return
                        scan(ray::rotate< +45_deg>(jumper)) |
                        scan(ray::rotate< -45_deg>(jumper)) |
                        scan(ray::rotate< +90_deg>(jumper)) |
                        scan(ray::rotate< -90_deg>(jumper)) |
                        scan(ray::rotate<+135_deg>(jumper)) |
                        scan(ray::rotate<-135_deg>(jumper))
                ;
        }

        template<class Iterator>
        auto scan(Iterator jumper) const
        {
                return is_en_prise(std::next(jumper));
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && builder.targets(jumper)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        auto add_jump(std::size_t dest_sq) const
        {
                builder.finish(dest_sq);
                builder.append_to(moves);
        }

        template<int Direction>
        static auto along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static auto is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
