#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/state/detail/i_token.hpp>   // token_interface
#include <dctl/core/state/dxp/version.hpp>      // protocol
#include <dctl/core/state/pdn/version.hpp>      // protocol

namespace dctl::core {
namespace detail {

template<class>
struct token_set;

template<>
struct token_set<dxp::protocol>
:
        public token_interface<'Z', 'W', 'e'>
{};

template<>
struct token_set<pdn::protocol>
:
        public token_interface<'B', 'W', '.'>
{
        constexpr static auto king  = 'K';
        constexpr static auto colon = ':';
        constexpr static auto comma = ',';
        constexpr static auto quote = '"';
};

}       // namespace detail
}       // namespace dctl::core
