#pragma once

//          Copyright Rein Halbersma 2010-2022.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {

[[nodiscard]] constexpr auto result(auto const& state, auto const& action) noexcept
{
        auto nrv(state); nrv.make(action); return nrv;
}

}       // namespace dctl::core
