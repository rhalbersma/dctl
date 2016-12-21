#pragma once
#include <dctl/state/position/bwk.hpp>          // Kingsrow (Ed Gilbert), Scan (Fabien Letouzey)
#include <dctl/state/position/bwke.hpp>         // Damy (Gerard Taille)
#include <dctl/state/position/c2p2e.hpp>        //
#include <dctl/state/position/cp22e.hpp>        // Damage (Bert Tuyt)
#include <dctl/state/position/wpo.hpp>          // Horizon (Feike Boomstra)
#include <experimental/optional>                // nullopt, optional

namespace dctl {

template<class Position, class... Args>
auto make_position(Args const&... args) noexcept
        -> std::experimental::optional<Position>
{
        return is_legal<board_t<Position>>(args...) ? Position(args...) : std::experimental::optional<Position>{};
}

}       // namespace dctl
