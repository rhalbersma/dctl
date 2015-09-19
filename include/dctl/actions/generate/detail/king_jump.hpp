#pragma once
#include <dctl/actions/detail/raii.hpp>                 // Launch, Capture, Visit, SetKingJump
#include <dctl/actions/detail/tracker.hpp>              // Tracker
#include <dctl/actions/generate/detail/primary_fwd.hpp> // Generate (primary template)
#include <dctl/actions/select/jump.hpp>                 // jump
#include <dctl/board/angle.hpp>                         // left_up, right_up, left_down, right_down, _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/board/ray.hpp>                           // make_iterator, rotate, mirror
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // king
#include <dctl/rule_traits.hpp>                         // is_orthogonal_jump_t, is_reversible_king_jump_direction_t, is_long_ranged_king_t,
                                                        // is_long_ranged_land_after_piece_t, is_halt_behind_final_king_t
#include <dctl/utility/type_traits.hpp>                 // board_t, rules_t
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // is_base_of, false_type, true_type

namespace dctl {
namespace actions {
namespace detail {

template<Color ToMove, class Reverse, class Tracker, class Sequence>
class Generate<ToMove, Piece::king, select::jump, Reverse, Tracker, Sequence>
{
        using board_type = board_t<Tracker>;
        using rules_type = rules_t<Tracker>;
        using   set_type =   set_t<Tracker>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, Reverse::value>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        Tracker& tracker;
        Sequence& moves;

public:
        Generate(Tracker& t, Sequence& m)
        :
                tracker{t},
                moves{m}
        {}

        auto operator()(set_type const& active_kings) const
        {
                raii::SetKingJump<Tracker> guard{tracker};
                serialize(active_kings);
        }

        template<class Iterator>
        auto try_next(Iterator jumper, passing_promotion_tag) const
        {
                static_assert(std::is_base_of<passing_promotion_tag, promotion_category_t<rules_type>>::value, "");
                assert(tracker.is_with(Piece::pawn) && tracker.is_into(Piece::king));
                try_next(jumper);
        }

private:
        auto serialize(set_type const& active_kings) const
        {
                active_kings.for_each([&](auto const& from_sq){
                        raii::Launch<Tracker> guard{tracker, from_sq};
                        branch(from_sq);
                });
        }

        auto branch(std::size_t from_sq) const
        {
                branch_dispatch(from_sq, is_orthogonal_jump_t<rules_type>{});
        }

        auto branch_dispatch(std::size_t from_sq, std::false_type) const
        {
                find_first(along_ray<left_up   (orientation)>(from_sq));
                find_first(along_ray<right_up  (orientation)>(from_sq));
                find_first(along_ray<left_down (orientation)>(from_sq));
                find_first(along_ray<right_down(orientation)>(from_sq));
        }

        auto branch_dispatch(std::size_t from_sq, std::true_type) const
        {
                find_first(along_ray<up        (orientation)>(from_sq));
                find_first(along_ray<left_up   (orientation)>(from_sq));
                find_first(along_ray<right_up  (orientation)>(from_sq));
                find_first(along_ray<left      (orientation)>(from_sq));
                find_first(along_ray<right     (orientation)>(from_sq));
                find_first(along_ray<left_down (orientation)>(from_sq));
                find_first(along_ray<right_down(orientation)>(from_sq));
                find_first(along_ray<down      (orientation)>(from_sq));
        }

        template<class Iterator>
        auto find_first(Iterator jumper) const
        {
                slide(jumper, tracker.template path<ray::direction_v<Iterator>>());
                if (is_onboard(jumper) && tracker.targets(jumper)) {
                        assert(is_onboard(std::next(jumper)));
                        capture(jumper);
                }
        }

        template<class Iterator>
        auto capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Capture<Tracker> guard{tracker, *jumper};
                land(std::next(jumper));
        }

        template<class Iterator>
        auto land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                raii::Visit<Tracker> guard{tracker, *jumper};
                try_next(jumper);
        }

        template<class Iterator>
        auto try_next(Iterator jumper) const
        {
                if (!find_next(jumper))
                        add(jumper);
        }

        template<class Iterator>
        auto find_next(Iterator jumper) const
        {
                return reverse_dispatch(jumper, is_reversible_king_jump_direction_t<rules_type>{});
        }

        // kings that cannot reverse their capture direction
        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, std::false_type) const
        {
                return scan_turn(jumper);
        }

        // kings that can reverse their capture direction
        template<class Iterator>
        auto reverse_dispatch(Iterator jumper, std::true_type) const
        {
                return scan_turn(jumper) | reverse(jumper);
        }

        template<class Iterator>
        auto reverse(Iterator jumper) const
        {
                static_assert(is_reversible_king_jump_direction_v<rules_type>, "");
                return scan(ray::rotate<180_deg>(jumper));
        }

        template<class Iterator>
        auto scan_turn(Iterator jumper) const
        {
                return scan_turn_dispatch(jumper, king_range_category_land_behind_piece_t<rules_type>{});
        }

        template<class Iterator>
        auto scan_turn_dispatch(Iterator jumper, short_ranged_tag) const
        {
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        auto scan_turn_dispatch(Iterator jumper, long_ranged_tag) const
        {
                // tracker.template path<Direction>() would be an ERROR here
                // because we need all landing squares rather than the directional launching squares subset
                assert(is_onboard(jumper) && tracker.path(*jumper));
                auto found_next = turn(jumper);
                auto slider = std::next(jumper);
                while (is_onboard(slider) && tracker.path(*slider)) {
                        tracker.set_dest(*slider);
                        found_next |= turn(slider);
                        ++slider;
                }
                tracker.set_dest(*jumper);
                return found_next |= is_en_prise(slider);
        }

        template<class Iterator>
        auto turn(Iterator jumper) const
        {
                return turn_dispatch(jumper, is_orthogonal_jump_t<rules_type>{});
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::false_type) const
        {
                static_assert(is_diagonal(direction_v<Iterator>), "");
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        template<class Iterator>
        auto turn_dispatch(Iterator jumper, std::true_type) const
        {
                static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>), "");
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
                slide(jumper, tracker.template path<ray::direction_v<Iterator>>());
                return is_en_prise(jumper);
        }

        template<class Iterator>
        auto slide(Iterator& jumper, set_type const& path) const
        {
                assert(is_onboard(jumper));
                slide_dispatch(jumper, path, king_range_category_t<rules_type>{});
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const& /* path */, short_ranged_tag) const
        {
                ++jumper;
        }

        template<class Iterator>
        auto slide_dispatch(Iterator& jumper, set_type const& path, long_ranged_tag) const
        {
                do ++jumper; while (is_onboard(jumper) && path.test(*jumper));
        }

        template<class Iterator>
        auto is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(jumper) && tracker.targets(jumper)))
                        return false;

                assert(is_onboard(std::next(jumper)));
                capture(jumper);
                return true;
        }

        template<class Iterator>
        auto add(Iterator dest_sq) const
        {
                halt_dispatch(dest_sq, king_range_category_land_behind_piece_t<rules_type>{}, king_range_category_halt_behind_king_t<rules_type>{});
        }

        template<class Iterator>
        auto halt_dispatch(Iterator dest_sq, long_ranged_tag, short_ranged_tag) const
        {
                if (tracker.is_last_jumped(Piece::king))
                        halt_dispatch(dest_sq, short_ranged_tag{}, short_ranged_tag{});
                else
                        halt_dispatch(dest_sq,  long_ranged_tag{},  long_ranged_tag{});
        }

        template<class Iterator>
        auto halt_dispatch(Iterator /* dest_sq */, short_ranged_tag, short_ranged_tag) const
        {
                add_jump();
        }

        template<class Iterator>
        auto halt_dispatch(Iterator dest_sq, long_ranged_tag, long_ranged_tag) const
        {
                // tracker.template path<Direction>() would be an ERROR here
                // because we need all halting squares rather than the directional launching squares subset
                assert(is_onboard(dest_sq) && tracker.path(*dest_sq));
                for (add_jump(), ++dest_sq; is_onboard(dest_sq) && tracker.path(*dest_sq); ++dest_sq) {
                        tracker.set_dest(*dest_sq);
                        add_jump();
                }
        }

        auto add_jump() const
        {
                tracker.append_to(moves);
        }

        template<int Direction>
        static auto along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }
};

}       // namespace detail
}       // namespace actions
}       // namespace dctl
