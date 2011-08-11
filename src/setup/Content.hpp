#include "Setup.h"
#include "../node/Material.h"

namespace dctl {
namespace setup {

template<typename Setup>
std::string content(const Material& p, int b)
{
        const BitBoard bb = BitBoard(1) << b;

        std::stringstream sstr;
        if (p.pieces(Side::BLACK) & bb) {
                if (p.kings() & bb)
			sstr << Setup::UPPER[Side::BLACK];      // black king
                else
                        sstr << Setup::LOWER[Side::BLACK];      // black man
        } else if (p.pieces(Side::WHITE) & bb) {
                if (p.kings() & bb)
                        sstr << Setup::UPPER[Side::WHITE];      // white king
                else
                        sstr << Setup::LOWER[Side::WHITE];      // white man
        } else
                sstr << Setup::EMPTY;                           // empty square
        return sstr.str();
}

}       // namespace setup
}       // namespace dctl
