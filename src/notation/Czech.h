#pragma once

namespace dctl {

namespace variant { struct Czech; }

namespace notation {

struct algebraic;
template<typename> struct Format;
template<> struct Format<variant::Czech>        { typedef algebraic type; };

}       // namespace notation
}       // namespace dctl
