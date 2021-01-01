#pragma once

//          Copyright Rein Halbersma 2010-2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

namespace dctl::core {
namespace block_adl {

// http://www.hijkendtc.nl/strategie-der-36-velden-hjb/
// http://www.dvuhhodki.narod.ru/simple30.html
struct micro
{
        constexpr static auto width = 6;
        constexpr static auto height = 6;
};

}       // namespace block_adl

using block_adl::micro;

}       // namespace dctl::core
