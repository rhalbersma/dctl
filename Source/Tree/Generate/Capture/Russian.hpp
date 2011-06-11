namespace variants { struct Russian; }

namespace tree {
namespace generate {
namespace capture {

template<>
struct init<variants::Russian>
{
        void operator()(Value& value) const
        {
                value.promotion = false;
        }
};

}       // namespace capture
}       // namespace generate
}       // namespace tree
