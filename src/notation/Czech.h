#pragma once

namespace dctl {

namespace rules { struct Czech; };

namespace notation {

struct algebraic;
template<typename> struct Format;
template<> struct Format<rules::Czech>          { typedef algebraic type; };

}       // namespace notation
}       // namespace dctl
