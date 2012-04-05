#pragma once
#include "Side.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Implementation>
struct PiecesInterface
{
public:
        // black or white pawns
        BitBoard pawns(bool color) const
        {
                return self()->do_pawns(color);
        }

        // black or white kings
        BitBoard kings(bool color) const
        {
                return self()->do_kings(color);
        }

        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return self()->do_pieces(color);
        }

        // black and white pawns
        BitBoard pawns() const
        {
                return self()->do_pawns();
        }

        // black and white kings
        BitBoard kings() const
        {
                return self()->do_kings();
        }

        // black and white pieces
        BitBoard pieces() const
        {
                return self()->do_pieces();
        }

        bool operator==(const PiecesInterface<Implementation>& other) const
        {
                return self()->equal(up_cast(other));
        }

        bool operator!=(const PiecesInterface<Implementation>& other) const
        {
                return !(*this == other);
        }

private:
        const Implementation& up_cast(const PiecesInterface<Implementation>& other) const
        {
                return static_cast<const Implementation&>(other);
        }

        const Implementation* self() const
        {
                return &up_cast(*this);
        }
};

}       // namespace dctl
