namespace Variants { struct Russian; }

namespace Capture {

template<>
struct init<Variants::Russian>
{
        void operator()(Value& value) const
        {
                value.promotion = false;
        }
};

}       // namespace Capture
