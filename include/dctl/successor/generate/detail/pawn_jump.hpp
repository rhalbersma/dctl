#pragma once
#include <dctl/angle.hpp>                               // _deg, rotate, inverse
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king, pawn
#include <dctl/position/promotion.hpp>                  // is_promotion
#include <dctl/successor/detail/raii.hpp>               // Launch, Capture, Visit, ToggleKingTargets, SetPromotion
#include <dctl/successor/detail/tracker.hpp>            // Tracker
#include <dctl/successor/generate/detail/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/detail/king_jump.hpp>        // promote_en_passant
#include <dctl/successor/select/jump.hpp>               // jumps

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/ray.hpp>                                 // make_iterator, rotate, mirror, turn
#include <dctl/rule_traits.hpp>                         // is_pawn_jump_king_t, is_backward_pawn_jump, is_orthogonal_jump_t, is_promotion_en_passant_t
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/iterator.hpp>                       // make_iterator
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type

namespace dctl {
namespace successor {
namespace detail {

template<Color ToMove, bool Reverse, class Position, class Sequence>
class Generate<ToMove, Piece::pawn, select::jump, Reverse, Position, Sequence>
{
        using    KingJumps = Generate<ToMove, Piece::king, select::jump, Reverse, Position, Sequence>;
        using   board_type = board_type_t<Position>;
        using   rules_type = rules_type_t<Position>;
        using     set_type =   set_type_t<Position>;
        using tracker_type = detail::Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        tracker_type& tracker;
        Sequence& moves;

public:
        explicit Generate(tracker_type& t, Sequence& m)
        :
                tracker{t},
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
                raii::ToggleKingTargets<tracker_type> guard{tracker};
                branch(active_pawns);
        }

        auto branch(set_type const& active_pawns) const
        {
                branch_dispatch(active_pawns, is_backward_pawn_jump_t<rules_type>{}, is_orthogonal_jump_t<rules_type>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::false_type, std::false_type) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        auto branch_dispatch(set_type const& active_pawns, std::true_type, std::false_type) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left_down (orientation)>(active_pawns);
                serialize<right_down(orientation)>(active_pawns);
        }

        // pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        auto branch_dispatch(set_type const& active_pawns, std::false_type, std::true_type) const
        {
                serialize<up        (orientation)>(active_pawns);
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left      (orientation)>(active_pawns);
                serialize<right     (orientation)>(active_pawns);
        }

        // pawns that jump in the 8 diagonal and orthogonal directions
        auto branch_dispatch(set_type const& active_pawns, std::true_type, std::true_type) const
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
                auto const jumpers = active_pawns & set_type(*std::prev(along_wave<Direction>(tracker.template targets<Direction>())));
                for (auto from_sq : jumpers)
                        find_first(along_ray<Direction>(from_sq));
        }

        template<class Iterator>
        auto find_first(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Launch<tracker_type> guard{tracker, *jumper};
                capture(std::next(jumper));
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Capture<tracker_type> guard{tracker, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Visit<tracker_type> guard{tracker, *jumper};
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
                add_jump();
        }

        template<class Iterator>
        auto try_promotion_dispatch(Iterator jumper, delayed_promotion_tag) const
        {
                if (is_promotion(*jumper))
                        return on_promotion(jumper);
                try_next(jumper);
        }

        template<class Iterator>
        auto on_promotion(Iterator jumper) const
        {
                raii::SetPromotion<tracker_type> guard{tracker};
                on_promotion_dispatch(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator /* jumper */, stopped_promotion_tag) const
        {
                add_jump();
        }

        template<class Iterator>
        auto on_promotion_dispatch(Iterator jumper, delayed_promotion_tag) const
        {
                try_next(jumper);
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
                raii::ToggleKingTargets<tracker_type> guard{tracker};
                king_jumps_try_next(jumper);
        }

        template<class Iterator>
        auto king_jumps_try_next(Iterator jumper) const
        {
                KingJumps{tracker, moves}.try_next(jumper, promotion_category_t<rules_type>{});
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!find_next(jumper))
                        add_jump();
        }

        template<class Iterator>
        auto find_next(Iterator jumper) const
        {
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                return turn_dispatch(jumper, is_backward_pawn_jump_t<rules_type>{}, is_orthogonal_jump_t<rules_type>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::false_type, std::false_type) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>), "");
                return scan(ray::mirror<up(orientation)>(jumper));
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::true_type, std::false_type) const
        {
                static_assert(is_diagonal(direction_v<Iterator>), "");
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        // pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::false_type, std::true_type) const
        {
                static_assert(!is_down(direction_v<Iterator>), "");
                return turn_dispatch(jumper, angle_t<direction_v<Iterator>>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_t<up(orientation)>) const
        {
                return
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_t<left_up(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_t<right_up(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_t<left(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, angle_t<right(orientation)>) const
        {
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        // pawns that jump in the 8 diagonal and orthogonal directions
        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::true_type, std::true_type) const
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
                if (!(is_onboard(std::next(jumper)) && tracker.targets(jumper)))
                        return false;

                capture(jumper);
                return true;
        }

        auto add_jump() const
        {
                moves.emplace_back(tracker);
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
}       // namespace successor
}       // namespace dctl
