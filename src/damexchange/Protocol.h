#pragma once

namespace damexchange {

// type traits
template<typename Protocol> struct port                 { static const unsigned short value = 27531; };
template<typename Protocol> struct terminator           { static const char           value =  '\0'; };
template<typename Protocol> struct header_length        { static const size_t         value =     1; };
template<typename Protocol> struct max_body_length      { static const size_t         value =   126; };

struct protocol_V1 {};                  // current version of the DamExchange protocol
typedef protocol_V1 protocol_tag;       // alias for the default protocol version

}       // namespace damexchange
