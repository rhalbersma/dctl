namespace Variant {

struct Russian;

template<>
struct init<Russian>
{
        void operator()(CaptureValue& capture) const
        {
                capture.promotion = false;
        }
};

}       // namespace Variant
