namespace Variant { struct Russian; }

namespace Capture {

template<>
struct init<Variant::Russian>
{
        void operator()(Value& value) const
        {
                value.promotion = false;
        }
};

}       // namespace Capture
