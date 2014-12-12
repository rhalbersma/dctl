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
        ~Launch()
        {
                tracker.finish();
        }

        Launch(Launch const&) = delete;
        Launch& operator=(Launch const&) = delete;

        Launch(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.launch(sq);
        }
};

template<class Tracker>
class Capture
{
        Tracker& tracker;
public:
        ~Capture()
        {
                tracker.release();
        }

        Capture(Capture const&) = delete;
        Capture& operator=(Capture const&) = delete;

        Capture(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.capture(sq);
        }
};

template<class Tracker>
class Visit
{
        Tracker& tracker;
public:
        ~Visit()
        {
                tracker.leave();
        }

        Visit(Visit const&) = delete;
        Visit& operator=(Visit const&) = delete;

        Visit(Tracker& t, std::size_t sq)
        :
                tracker{t}
        {
                tracker.visit(sq);
        }
};

template<class Tracker>
class ToggleKingTargets
{
        Tracker& tracker;
public:
        ~ToggleKingTargets()
        {
                tracker.toggle_king_targets();
        }

        ToggleKingTargets(ToggleKingTargets const&) = delete;
        ToggleKingTargets& operator=(ToggleKingTargets const&) = delete;

        ToggleKingTargets(Tracker& t)
        :
                tracker{t}
        {
                tracker.toggle_king_targets();
        }
};

template<class Tracker>
class ToggleIsWithKing
{
        Tracker& tracker;
public:
        ~ToggleIsWithKing()
        {
                assert(tracker.is_with_king());
                tracker.toggle_is_with_king();
        }

        ToggleIsWithKing(ToggleIsWithKing const&) = delete;
        ToggleIsWithKing& operator=(ToggleIsWithKing const&) = delete;

        ToggleIsWithKing(Tracker& t)
        :
                tracker{t}
        {
                assert(!tracker.is_with_king());
                tracker.toggle_is_with_king();
        }
};

template<class Tracker>
class ToggleIsPromotion
{
        Tracker& tracker;
public:
        ~ToggleIsPromotion()
        {
                assert(tracker.is_promotion());
                tracker.toggle_is_promotion();
        }

        ToggleIsPromotion(ToggleIsPromotion const&) = delete;
        ToggleIsPromotion& operator=(ToggleIsPromotion const&) = delete;

        ToggleIsPromotion(Tracker& t)
        :
                tracker{t}
        {
                assert(!tracker.is_promotion());
                tracker.toggle_is_promotion();
        }
};

}       // namespace raii
}       // namespace successor
}       // namespace dctl
