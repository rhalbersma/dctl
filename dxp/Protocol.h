#pragma once

namespace dctl {
namespace dxp {

// type traits
template<typename> struct port                  { static const unsigned short value = 27531; };
template<typename> struct terminator            { static const char           value =  '\0'; };
template<typename> struct header_length         { static const int            value =     1; };
template<typename> struct max_body_length       { static const int            value =   126; };

}       // namespace dxp
}       // namespace dctl
