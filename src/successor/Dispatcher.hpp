#pragma once
#include <array>                        // array
#include <functional>                   // function
#include "Driver.hpp"
#include "Result.hpp"
#include "../node/Material.hpp"
#include "../node/Side.hpp"
#include "../node/Stack.hpp"
#include "../utility/nonconstructible.hpp"

namespace dctl {
namespace successor {
namespace detail {

// TODO: use C++11 variadic templates
template<typename, typename>
struct signature;

template<typename Position>
struct signature<generation, Position>
{
        //typedef std::function<void(Position const&, Stack&)> type;
        typedef void (* type)(Position const&, Stack&);
};

template<typename Position>
struct signature<enumeration, Position>
{
        //typedef std::function<int(Position const&)> type;
        typedef int (* type)(Position const&);
};

template<typename Position>
struct signature<detection, Position>
{
        //typedef std::function<bool(Position const&)> type;
        typedef bool (* type)(Position const&);
};

// TODO: use C++11 variadic templates
template<bool, int, typename, typename, template<typename, typename> class, typename, typename>
struct call_back;

template
<
        bool Color, int Material, typename Selection, 
        template<typename, typename> class Position, typename Rules, typename Board
>
struct call_back<Color, Material, Selection, generation, Position, Rules, Board>
{
        static void run(Position<Rules, Board> const& p, Stack& moves)
        {
                Driver<Color, Material, Selection, generation, Rules, Board>::generate(p, moves);
        }
};

template
<
        bool Color, int Material, typename Selection, 
        template<typename, typename> class Position, typename Rules, typename Board
>
struct call_back<Color, Material, Selection, enumeration, Position, Rules, Board>
{
        static int run(Position<Rules, Board> const& p)
        {
                return Driver<Color, Material, Selection, enumeration, Rules, Board>::count(p);
        }
};

template
<
        bool Color, int Material, typename Selection, 
        template<typename, typename> class Position, typename Rules, typename Board
>
struct call_back<Color, Material, Selection, detection, Position, Rules, Board>
{
        static bool run(Position<Rules, Board> const& p)
        {
                return Driver<Color, Material, Selection, detection, Rules, Board>::detect(p);
        }
};

}       // namespace detail

template
<
        typename Selection,
        typename Result,
        template<typename, typename> class Position,
        typename Rules,
        typename Board
>
struct Dispatcher
:
        // enforce static semantics
        private nonconstructible
{
private:
        // TODO: use C++11 template aliases
        template<bool Color, int Material>
        struct Delegate
        :
                detail::call_back<Color, Material, Selection, Result, Position, Rules, Board>
        {};

public:
        // typedefs

        typedef typename detail::signature< Result, Position<Rules, Board> >::type Functor;

        static Functor const select(int state /* partial intrinsic state encoding */ )
        {
                // full intrinsic state representations

                // "Meyers Singleton", Effective C++ 3rd ed., Item 4 (p. 31-32)
                static Functor const singleton_[] = {
                        &Delegate<Side::black, Material::none>::run,
                        &Delegate<Side::black, Material::pawn>::run,
                        &Delegate<Side::black, Material::king>::run,
                        &Delegate<Side::black, Material::both>::run,
                        &Delegate<Side::white, Material::none>::run,
                        &Delegate<Side::white, Material::pawn>::run,
                        &Delegate<Side::white, Material::king>::run,
                        &Delegate<Side::white, Material::both>::run
                };

                return singleton_[state];
        }
};

}       // namespace successor
}       // namespace dctl
