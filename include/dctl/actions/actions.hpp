#pragma once
#include <dctl/actions/detail/specializations.hpp>      // generate
#include <dctl/actions/select/legal.hpp>                // legal
#include <dctl/color_piece.hpp>                         // black, white
#include <dctl/rule_traits.hpp>
#include <dctl/utility/type_traits.hpp>                 // value_t
#include <cassert>                                      // assert
#include <type_traits>                                  // bool_constant, is_same

namespace dctl {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class Actions
{
        template<class color>
        using Impl = detail::Actions<color, Select, DuplicatesPolicy, std::bool_constant<Reverse>>;

        template<class color, class State, class SequenceContainer>
        auto assert_invariants([[maybe_unused]] State const& s, [[maybe_unused]] SequenceContainer const& actions) const
        {
                [[maybe_unused]] auto const n = static_cast<int>(actions.size());
                assert( count<color>(s) ==  n     );
                assert(detect<color>(s) == (n > 0));
        }

public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
                return
                        s.to_move() == color::black ?
                        generate<black_>(s, a) :
                        generate<white_>(s, a)
                ;
        }

        template<class Color, class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
                using action_type = value_t<SequenceContainer>;
                static_assert(std::is_same<rules_t<State>, rules_t<action_type>>{});
                static_assert(std::is_same<board_t<State>, board_t<action_type>>{});
                Impl<Color>{}.generate(s, a);
                assert_invariants<Color>(s, a);
        }

        template<class State>
        auto count(State const& s) const
        {
                return
                        s.to_move() == color::black ?
                        count<black_>(s) :
                        count<white_>(s)
                ;
        }

        template<class Color, class State>
        auto count(State const& s) const
        {
                return Impl<Color>{}.count(s);
        }

        template<class State>
        auto detect(State const& s) const
        {
                return
                        s.to_move() == color::black ?
                        detect<black_>(s) :
                        detect<white_>(s)
                ;
        }

        template<class Color, class State>
        auto detect(State const& s) const
        {
                return Impl<Color>{}.detect(s);
        }
};

}       // namespace dctl
