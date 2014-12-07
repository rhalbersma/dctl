#pragma once
#include <dctl/angle.hpp>                               // _deg, rotate, inverse
#include <dctl/color.hpp>                               // Color
#include <dctl/piece.hpp>                               // PieceKingType, PiecePawnType
#include <dctl/position/promotion.hpp>                  // is_promotion
#include <dctl/successor/generate/primary_fwd.hpp>      // Generate (primary template)
#include <dctl/successor/generate/king_jump.hpp>        // promote_en_passant
#include <dctl/successor/tracker.hpp>                   // Tracker
#include <dctl/successor/select/jump.hpp>               // jumps

#include <dctl/board/orientation.hpp>                   // orientation_v
#include <dctl/ray.hpp>                                 // make_iterator, rotate, mirror, turn
#include <dctl/rule_traits.hpp>
#include <dctl/type_traits.hpp>                         // board_type_t, rules_type_t, set_type_t
#include <dctl/wave/iterator.hpp>
#include <cassert>                                      // assert
#include <iterator>                                     // prev
#include <type_traits>                                  // false_type, true_type
#include <utility>                                      // pair

namespace dctl {
namespace successor {

template<Color ToMove, bool IsReverse, class Position, class Sequence>
class Generate<ToMove, IsReverse, PiecePawnType, select::jump, Position, Sequence>
{
        using KingJumps = Generate<ToMove, IsReverse, PieceKingType, select::jump, Position, Sequence>;
        using board_type = board_type_t<Position>;
        using rules_type = rules_type_t<Position>;
        using   set_type =   set_type_t<Position>;
        using State = Tracker<ToMove, Position>;

        static constexpr auto orientation = orientation_v<board_type, ToMove, IsReverse>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        State& tracker;
        Sequence& moves;

public:
        explicit Generate(State& t, Sequence& m)
        :
                tracker{t},
                moves{m}
        {}

        void operator()(set_type const& active_pawns) const
        {
                if (active_pawns.none())
                        return;

                // tag dispatching on whether pawns can capture kings
                king_targets_dispatch(active_pawns, is_pawn_jump_king_t<rules_type>{});
        }

private:
        // pawns that can capture kings
        void king_targets_dispatch(set_type const& active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        // pawns that cannot capture kings
        void king_targets_dispatch(set_type const& active_pawns, std::false_type) const
        {
                tracker.toggle_king_targets();
                branch(active_pawns);
                tracker.toggle_king_targets();
        }

        void branch(set_type const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, std::pair<is_backward_pawn_jump_t<rules_type>, is_orthogonal_jump_t<rules_type>>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        void branch_dispatch(set_type const& active_pawns, std::pair<std::false_type, std::false_type>) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        void branch_dispatch(set_type const& active_pawns, std::pair<std::true_type, std::false_type>) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left_down (orientation)>(active_pawns);
                serialize<right_down(orientation)>(active_pawns);
        }

        // pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        void branch_dispatch(set_type const& active_pawns, std::pair<std::false_type, std::true_type>) const
        {
                serialize<up        (orientation)>(active_pawns);
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left      (orientation)>(active_pawns);
                serialize<right     (orientation)>(active_pawns);
        }

        // pawns that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(set_type const& active_pawns, std::pair<std::true_type, std::true_type>) const
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
        void serialize(set_type const& active_pawns) const
        {
                auto const jumpers = active_pawns & set_type(*std::prev(along_wave<Direction>(tracker.template targets<Direction>())));
                for (auto&& from_sq : jumpers)
                        find_first(along_ray<Direction>(from_sq));
        }

        template<class Iterator>
        void find_first(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                tracker.launch(*jumper);
                capture(std::next(jumper));
                tracker.finish();
        }

        template<class Iterator>
        void capture(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                tracker.capture(*jumper);
                land(std::next(jumper));
                tracker.release();
        }

        template<class Iterator>
        void land(Iterator jumper) const
        {
                assert(is_onboard(jumper));
                tracker.visit(*jumper);
                find_next(jumper);
                tracker.leave();
        }

        template<class Iterator>
        void find_next(Iterator jumper) const
        {
                // tag dispatching on promotion condition
                promotion_dispatch(jumper, is_en_passant_promotion_t<rules_type>{});
        }

        // pawns that promote apres-fini
        template<class Iterator>
        void promotion_dispatch(Iterator jumper, std::false_type) const
        {
                if (explore(jumper))
                        return;

                if (is_promotion(*jumper)) {
                        tracker.toggle_is_promotion();
                        add_jump();
                        tracker.toggle_is_promotion();
                } else {
                        add_jump();
                }
        }

        // pawns that promote en-passant
        template<class Iterator>
        void promotion_dispatch(Iterator jumper, std::true_type) const
        {
                if (is_promotion(*jumper)) {
                        tracker.toggle_is_promotion();
                        if (!KingJumps{tracker, moves}.promote_en_passant(jumper))
                                add_jump();
                        tracker.toggle_is_promotion();
                } else {
                        if (!explore(jumper))
                                add_jump();
                }
        }

        template<class Iterator>
        bool explore(Iterator jumper) const
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on pawn turn directions
                return turn_dispatch(jumper, std::pair<is_backward_pawn_jump_t<rules_type>, is_orthogonal_jump_t<rules_type>>{});
        }

        // pawns that jump in the 2 forward diagonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::false_type, std::false_type>) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>), "");
                return scan(ray::mirror<up(orientation)>(jumper));
        }

        // pawns that jump in the 4 forward and backward diagonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::true_type, std::false_type>) const
        {
                static_assert(is_diagonal(direction_v<Iterator>), "");

                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::rotate<+90_deg>(jumper)) |
                        scan(ray::rotate<-90_deg>(jumper))
                ;
        }

        // pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::false_type, std::true_type>) const
        {
                static_assert(!is_down(direction_v<Iterator>) && (is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>)), "");

                // tag dispatching on the current jump direction
                return turn_dispatch(jumper, angle_t<direction_v<Iterator>>{});
        }

        // the upward direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, angle_t<up(orientation)>)
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        // the left upward direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, angle_t<left_up(orientation)>)
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        // the right upward direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, angle_t<right_up(orientation)>)
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<left    (orientation)>(jumper)) |
                        scan(ray::turn<right   (orientation)>(jumper))
                ;
        }

        // the left direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, angle_t<left(orientation)>)
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        // the right direction
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, angle_t<right(orientation)>)
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return
                        scan(ray::turn<up      (orientation)>(jumper)) |
                        scan(ray::turn<left_up (orientation)>(jumper)) |
                        scan(ray::turn<right_up(orientation)>(jumper))
                ;
        }

        // pawns that jump in the 8 diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::true_type, std::true_type>) const
        {
                static_assert(is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>), "");

                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
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
        bool scan(Iterator jumper) const
        {
                return is_en_prise(std::next(jumper));
        }

        template<class Iterator>
        bool is_en_prise(Iterator jumper) const
        {
                if (!(is_onboard(std::next(jumper)) && tracker.targets(jumper)))
                        return false;

                capture(jumper);
                return true;
        }

        void add_jump() const
        {
                moves.emplace_back(tracker);
        }

        template<int Direction>
        static wave::Iterator<board_type, Direction> along_wave(set_type const& s)
        {
                return wave::make_iterator<board_type, Direction>(s);
        }

        template<int Direction>
        static ray::Iterator<board_type, Direction> along_ray(std::size_t sq)
        {
                return ray::make_iterator<board_type, Direction>(sq);
        }

        static bool is_promotion(std::size_t sq)
        {
                return dctl::is_promotion<board_type, ToMove>(sq);
        }
};

}       // namespace successor
}       // namespace dctl
