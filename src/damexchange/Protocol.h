#pragma once

namespace damexchange {

template<typename> struct port;
template<typename> struct terminator;
template<typename> struct header_length;
template<typename> struct max_body_length;

struct DXP_tag {};

template<> struct port<DXP_tag>                 { static const unsigned short value = 27531; };
template<> struct terminator<DXP_tag>           { static const char           value =  '\0'; };
template<> struct header_length<DXP_tag>        { static const size_t         value =     1; };
template<> struct max_body_length<DXP_tag>      { static const size_t         value =   126; };

}       // namespace damexchange
