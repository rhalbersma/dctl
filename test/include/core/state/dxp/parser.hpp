#pragma once

//          Copyright Rein Halbersma 2010-2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <dctl/core/ui/dxp/message.hpp> // parser
#include <boost/blank.hpp>              // blank
#include <string>                       // string
#include <vector>                       // vector

struct Fixture
{
        dctl::core::dxp::parser parse;

        struct visitor
        {
                std::vector<std::string>& data;

                template<class T>
                auto operator()(T const& value) { data.push_back(value.str()); }

                auto operator()(boost::blank) {}
        };

        std::vector<std::string> parsed;
        visitor vis{parsed};
};
