#pragma once
#include <dctl/core/actions/detail/specializations.hpp> // generate
#include <dctl/core/actions/select/legal.hpp>           // legal
#include <dctl/core/state/color_piece.hpp>              // black, white
#include <dctl/core/rules/traits.hpp>
#include <dctl/util/type_traits.hpp>                    // value_t
#include <cassert>                                      // assert
#include <type_traits>                                  // bool_constant, is_same

namespace dctl {
namespace core {

template<class Select = select::legal, class DuplicatesPolicy = drop_duplicates_tag, bool Reverse = false>
class Actions
{
        template<class Color>
        static constexpr auto Impl = detail::Actions<Color, Select, DuplicatesPolicy, std::bool_constant<Reverse>>{};

        template<class Color, class State, class SequenceContainer>
        auto assert_invariants([[maybe_unused]] State const& s, [[maybe_unused]] SequenceContainer const& actions) const
        {
                [[maybe_unused]] auto const n = static_cast<int>(actions.size());
                assert( count<Color>(s) ==  n     );
                assert(detect<Color>(s) == (n > 0));
        }

public:
        template<class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
                return s.to_move() == color::black ? generate<black_>(s, a) : generate<white_>(s, a);
        }

        template<class Color, class State, class SequenceContainer>
        auto generate(State const& s, SequenceContainer& a) const
        {
                using action_type = value_t<SequenceContainer>;
                static_assert(std::is_same_v<rules_t<State>, rules_t<action_type>>);
                static_assert(std::is_same_v<board_t<State>, board_t<action_type>>);
                Impl<Color>.generate(s, a);
                assert_invariants<Color>(s, a);
        }

        template<class State>
        auto count(State const& s) const
        {
                return s.to_move() == color::black ? count<black_>(s) : count<white_>(s);
        }

        template<class Color, class State>
        auto count(State const& s) const
        {
                return Impl<Color>.count(s);
        }

        template<class State>
        auto detect(State const& s) const
        {
                return s.to_move() == color::black ? detect<black_>(s) : detect<white_>(s);
        }

        template<class Color, class State>
        auto detect(State const& s) const
        {
                return Impl<Color>.detect(s);
        }
};

constexpr auto keep_duplicates_gen = Actions<select::legal, keep_duplicates_tag>{};
constexpr auto drop_duplicates_gen = Actions<select::legal, drop_duplicates_tag>{};

}       // namespace core
}       // namespace dctl
