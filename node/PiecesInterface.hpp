#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Implementation>
struct PiecesInterface
{
public:
        // black or white pawns
        BitBoard pawns(bool color) const
        {
                return self().do_pawns(color);
        }

        // black or white kings
        BitBoard kings(bool color) const
        {
                return self().do_kings(color);
        }

        // black or white pieces
        BitBoard pieces(bool color) const
        {
                return self().do_pieces(color);
        }

        // black and white pawns
        BitBoard pawns() const
        {
                return self().do_pawns();
        }

        // black and white kings
        BitBoard kings() const
        {
                return self().do_kings();
        }

        // black and white pieces
        BitBoard pieces() const
        {
                return self().do_pieces();
        }

        // delegate to a derived class
        bool operator==(const PiecesInterface<Implementation>& other) const
        {
                return self().equal(down_cast(other));
        }

        // delegate to the operator==
        bool operator!=(const PiecesInterface<Implementation>& other) const
        {
                return !(*this == other);
        }

private:
        // cast a base reference to a derived reference (i.e. "down" the class hierarchy)
        const Implementation& down_cast(const PiecesInterface<Implementation>& other) const
        {
                return static_cast<const Implementation&>(other);
        }

        // cast the this pointer to a derived reference
        const Implementation& self() const
        {
                return down_cast(*this);
        }
};

}       // namespace dctl
