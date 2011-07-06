#include "../Tree/Generate/Capture/Templates.h"
#include "../Tree/Generate/Capture/Value.h"

namespace tree {
namespace generate {
namespace capture {

template<>
struct init<rules::Russian>
{
        void operator()(Value& value) const
        {
                value.promotion = false;
        }
};

}       // namespace capture
}       // namespace generate
}       // namespace tree
