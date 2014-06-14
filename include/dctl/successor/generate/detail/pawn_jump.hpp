#pragma once
#include <dctl/successor/generate/detail/primary_fwd.hpp>       // Generate (primary template)
#include <dctl/successor/generate/detail/king_jump.hpp>         // promote_en_passant
#include <dctl/successor/propagate/jump.hpp>                    // Propagate (jumps specialization)
#include <dctl/successor/select/jump.hpp>                       // jumps
#include <dctl/pieces/pawn.hpp>                                 // pawn
#include <dctl/pieces/king.hpp>

#include <dctl/angle.hpp>                                       // _deg, rotate, inverse
#include <dctl/board/orientation.hpp>                           // orientation_v
#include <dctl/position/promotion.hpp>
#include <dctl/rules/traits.hpp>
#include <dctl/utility/algorithm.hpp>
#include <dctl/ray.hpp>                                         // make_iterator, rotate, mirror, turn
#include <dctl/wave/iterator.hpp>
#include <cassert>                                              // assert
#include <iterator>                                             // prev
#include <type_traits>                                          // false_type, true_type
#include <utility>                                              // pair

namespace dctl {
namespace successor {
namespace detail {

// partial specialization for pawn jumps generation
template<bool Color, class Position, class Sequence>
struct Generate<Color, pieces::pawn, select::jump, Position, Sequence>
{
public:
        // enforce reference semantics
        Generate(Generate const&) = delete;
        Generate& operator=(Generate const&) = delete;

private:
        using KingJumps = Generate<Color, pieces::king, select::jump, Position, Sequence>;
        using Rules = typename Position::rules_type;
        using Board = typename Position::board_type;
        using Set = typename Board::set_type;
        using State = Propagate<select::jump, Position>;

        static constexpr auto orientation = orientation_v<Board, Color>;

        template<class Iterator>
        static constexpr auto direction_v = rotate(ray::direction_v<Iterator>, inverse(orientation));

        // representation

        State& capture_;
        Sequence& moves_;

public:
        // structors

        explicit Generate(State& c, Sequence& m)
        :
                capture_{c},
                moves_{m}
        {}

        // function call operators

        void operator()(Set const& active_pawns) const
        {
                if (active_pawns.empty())
                        return;

                // tag dispatching on whether pawns can capture kings
                select_dispatch(active_pawns, rules::can_jump<Rules, pieces::pawn, pieces::king>{});
        }

private:
        // overload for pawns that can capture kings
        void select_dispatch(Set const& active_pawns, std::true_type) const
        {
                branch(active_pawns);
        }

        // overload for pawns that cannot capture kings
        void select_dispatch(Set const& active_pawns, std::false_type) const
        {
                capture_.toggle_king_targets();
                branch(active_pawns);
                capture_.toggle_king_targets();
        }

        void branch(Set const& active_pawns) const
        {
                // tag dispatching on pawn jump directions
                branch_dispatch(active_pawns, std::pair<is_backward_pawn_jump_t<Rules>, is_orthogonal_jump_t<Rules>>{});
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        void branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::false_type>) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
        }

        // overload for pawns that jump in the 4 forward and backward diagonal directions
        void branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::false_type>) const
        {
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left_down (orientation)>(active_pawns);
                serialize<right_down(orientation)>(active_pawns);
        }

        // overload for pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        void branch_dispatch(Set const& active_pawns, std::pair<std::false_type, std::true_type>) const
        {
                serialize<up        (orientation)>(active_pawns);
                serialize<left_up   (orientation)>(active_pawns);
                serialize<right_up  (orientation)>(active_pawns);
                serialize<left      (orientation)>(active_pawns);
                serialize<right     (orientation)>(active_pawns);
        }

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
        void branch_dispatch(Set const& active_pawns, std::pair<std::true_type, std::true_type>) const
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
        void serialize(Set const& active_pawns) const
        {
                auto const jumpers = active_pawns & *std::prev(along_wave<Direction>(capture_.template targets_with_pawn<Direction>()));
                for (auto&& from_sq : jumpers)
                        find(along_ray<Direction>(from_sq));
        }

        template<class Iterator>
        void find(Iterator jumper) const
        {
                capture_.launch(*jumper);
                explore(std::next(jumper));     // recursively find more jumps
                capture_.finish(*jumper);
        }

        template<class Iterator>
        void explore(Iterator jumper) const
        {
                capture_.make(*jumper);
                add_and_continue(std::next(jumper));
                capture_.undo(*jumper);
        }

        template<class Iterator>
        void add_and_continue(Iterator jumper) const
        {
                // tag dispatching on majority precedence
                precedence_dispatch(jumper, rules::is_precedence<Rules>{});
        }

        // overload for no majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::false_type) const
        {
                if (is_finished(jumper))
                        add(jumper);
        }

        // overload for majority precedence
        template<class Iterator>
        void precedence_dispatch(Iterator jumper, std::true_type) const
        {
                if (is_finished(jumper) && capture_.greater_equal()) {
                        if (capture_.not_equal_to()) {
                                capture_.improve();
                                moves_.clear();
                        }
                        add(jumper);
                }
        }

        template<class Iterator>
        bool is_finished(Iterator jumper) const
        {
                // tag dispatching on promotion condition
                return !promotion_dispatch(jumper, is_en_passant_promotion_t<Rules>{});
        }

        // overload for pawns that promote apres-fini
        template<class Iterator>
        bool promotion_dispatch(Iterator jumper, std::false_type) const
        {
                return find_next(jumper);
        }

        // overload for pawns that promote en-passant
        template<class Iterator>
        bool promotion_dispatch(Iterator jumper, std::true_type) const
        {
                return is_promotion(*jumper) ? promote_en_passant(jumper) : find_next(jumper);
        }

        template<class Iterator>
        bool promote_en_passant(Iterator jumper) const
        {
                // tag dispatching on whether pawns can capture kings
                return can_jump_dispatch(jumper, rules::can_jump<Rules, pieces::pawn, pieces::king>{});
        }

        // overload for pawns that can capture kings
        template<class Iterator>
        bool can_jump_dispatch(Iterator jumper, std::true_type) const
        {
                capture_.toggle_promotion();
                auto const found_next = KingJumps{capture_, moves_}.promote_en_passant(jumper);
                capture_.toggle_promotion();
                return found_next;
        }

        // overload for pawns that cannot capture kings
        template<class Iterator>
        bool can_jump_dispatch(Iterator jumper, std::false_type) const
        {
                capture_.toggle_promotion();    // no longer a pawn
                capture_.set_king_targets();    // can now capture kings
                auto const found_next = KingJumps{capture_, moves_}.promote_en_passant(jumper);
                capture_.clear_king_targets();  // can no longer capture kings
                capture_.toggle_promotion();    // now a pawn again
                return found_next;
        }

        template<class Iterator>
        bool find_next(Iterator jumper) const
        {
                // CORRECTNESS: bitwise instead of logical OR to disable short-circuiting
                return scan(jumper) | turn(jumper);
        }

        template<class Iterator>
        bool turn(Iterator jumper) const
        {
                // tag dispatching on pawn turn directions
                return turn_dispatch(jumper, std::pair<is_backward_pawn_jump_t<Rules>, is_orthogonal_jump_t<Rules>>{});
        }

        // overload for pawns that jump in the 2 forward diagonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::false_type, std::false_type>) const
        {
                static_assert(is_up(direction_v<Iterator>) && is_diagonal(direction_v<Iterator>), "");
                return scan(ray::mirror<up(orientation)>(jumper));
        }

        // overload for pawns that jump in the 4 forward and backward diagonal directions
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

        // overload for pawns that jump in the 5 forward and sideways diagonal and orthogonal directions
        template<class Iterator>
        bool turn_dispatch(Iterator jumper, std::pair<std::false_type, std::true_type>) const
        {
                static_assert(!is_down(direction_v<Iterator>) && (is_diagonal(direction_v<Iterator>) || is_orthogonal(direction_v<Iterator>)), "");

                // tag dispatching on the current jump direction
                return turn_dispatch(jumper, angle_t<direction_v<Iterator>>{});
        }

        // overload for the upward direction
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

        // overload for the left upward direction
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

        // overload for the right upward direction
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

        // overload for the left direction
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

        // overload for the right direction
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

        // overload for pawns that jump in the 8 diagonal and orthogonal directions
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
                if (!capture_.targets_with_pawn(jumper))
                        return false;

                explore(jumper);        // recursively find more jumps
                return true;
        }

        template<class Iterator>
        void add(Iterator dest) const
        {
                // tag dispatching on ambiguity of pawn jumps
                ambiguity_dispatch(dest, rules::is_unambiguous_pawn_jump<Rules>{});
        }

        // overload for pawn jumps that are always unambiguous
        template<class Iterator>
        void ambiguity_dispatch(Iterator dest, std::true_type) const
        {
                capture_.template add_pawn_jump<Color, with::pawn>(*dest, moves_);
        }

        // overload for pawn jumps that are potentially ambiguous
        template<class Iterator>
        void ambiguity_dispatch(Iterator dest, std::false_type) const
        {
                auto const check_duplicate = rules::is_remove_duplicates<Rules>::value && capture_.is_potential_duplicate(moves_);
                capture_.template add_pawn_jump<Color, with::pawn>(*dest, moves_);
                if (check_duplicate && util::is_duplicate_back(moves_))
                        moves_.pop_back();
        }

        template<int Direction>
        static wave::Iterator<Board, Direction> along_wave(Set const& s)
        {
                return wave::make_iterator<Board, Direction>(s);
        }

        template<int Direction>
        static ray::Iterator<Board, Direction> along_ray(int sq)
        {
                return ray::make_iterator<Board, Direction>(sq);
        }

        static bool is_promotion(int sq)
        {
                return dctl::is_promotion<Color, Board>(sq);
        }
};

}       // namespace detail
}       // namespace successor
}       // namespace dctl
