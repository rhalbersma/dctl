#pragma once

namespace dctl {
namespace successor {

template<class Move>
class Precedence
{
public:
        template<class Sequence>
        auto operator()(Sequence& moves, Move const& m)
        {
                auto const current = Value{m};

                if (moves.empty())      goto START1;
                if (max_ <  current)    goto START0;
                if (max_ == current)    goto START2;

                return false;

                START0 : moves.clear();
                START1 : max_ = current;
                START2 : return true;
        }

private:
        using Value = typename Move::Rules::template value_type<Move>;
        Value max_{};
};

}       // namespace successor
}       // namespace dctl
