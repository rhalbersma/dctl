namespace Variants { struct Russian; }

namespace Tree {
namespace Generate {
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
}       // namespace Generate
}       // namespace Tree
