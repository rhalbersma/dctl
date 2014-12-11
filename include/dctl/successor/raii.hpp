#pragma once
#include <cassert>      // assert
#include <cstddef>      // size_t

namespace dctl {
namespace successor {
namespace raii {

template<class Tracker>
class Launch
{
        Tracker& tracker;
public:
        Launch(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.launch(sq);
        }

        ~Launch()
        {
                tracker.finish();
        }
};

template<class Tracker>
class Capture
{
        Tracker& tracker;
public:
        Capture(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.capture(sq);
        }

        ~Capture()
        {
                tracker.release();
        }
};

template<class Tracker>
class Visit
{
        Tracker& tracker;
public:
        Visit(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.visit(sq);
        }

        ~Visit()
        {
                tracker.leave();
        }
};

template<class Tracker>
class ToggleKingTargets
{
        Tracker& tracker;
public:
        ToggleKingTargets(Tracker& t)
        :
                tracker{t}
        {
                tracker.toggle_king_targets();
        }

        ~ToggleKingTargets()
        {
                tracker.toggle_king_targets();
        }
};

template<class Tracker>
class ToggleIsWithKing
{
        Tracker& tracker;
public:
        ToggleIsWithKing(Tracker& t)
        :
                tracker{t}
        {
                assert(!tracker.is_with_king());
                tracker.toggle_is_with_king();
        }

        ~ToggleIsWithKing()
        {
                assert(tracker.is_with_king());
                tracker.toggle_is_with_king();
        }
};

template<class Tracker>
class ToggleIsPromotion
{
        Tracker& tracker;
public:
        ToggleIsPromotion(Tracker& t)
        :
                tracker{t}
        {
                assert(!tracker.is_promotion());
                tracker.toggle_is_promotion();
        }

        ~ToggleIsPromotion()
        {
                assert(tracker.is_promotion());
                tracker.toggle_is_promotion();
        }
};

}       // namespace raii
}       // namespace successor
}       // namespace dctl
