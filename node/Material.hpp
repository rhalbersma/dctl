#pragma once
#include <boost/operators.hpp>          // equality_comparable
#include "PiecesInterface.hpp"
#include "Move.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

struct Material
:
        // Curiously Recurring Template Pattern (CRTP)
        public PiecesInterface<Material>,
        private boost::equality_comparable<Material>
{
        enum {
                none = 0,
                pawn = 1,
                king = pawn << 1,
                both = pawn ^ king
        };

        // default constructor
        Material()
        {
                // no-op
        }

        // zero initialize
        explicit Material(BitBoard /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                BOOST_ASSERT(invariant());
        }

        // initialize with a set of bitboards
        Material(BitBoard black_pieces, BitBoard white_pieces, BitBoard kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                BOOST_ASSERT(invariant());
        }

        bool operator==(const Material& other) const
        {
                return (
                        (pieces(Side::black) == other.pieces(Side::black)) &&
                        (pieces(Side::white) == other.pieces(Side::white)) &&
                                    (kings() == other.kings())
                );
        }

        // xor-assign the set bits of another piece set
        Material& operator^=(const Move& m)
        {
                pieces_[Side::black] ^= m.pieces(Side::black);
                pieces_[Side::white] ^= m.pieces(Side::white);
                kings_ ^= m.kings();
                BOOST_ASSERT(invariant());
                return *this;
        }

private:
        friend struct PiecesInterface<Material>;

        // black or white pawns
        BitBoard do_pawns(bool color) const
        {
                return do_pieces(color) & ~do_kings();
        }

        // black or white kings
        BitBoard do_kings(bool color) const
        {
                return do_pieces(color) & do_kings();
        }

        // black or white pieces
        BitBoard do_pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        BitBoard do_pawns() const
        {
                return do_pieces() & ~do_kings();
        }

        // black and white kings
        BitBoard do_kings() const
        {
                return kings_;
        }

        // black and white pieces
        BitBoard do_pieces() const
        {
                return do_pieces(Side::black) ^ do_pieces(Side::white);
        }

        // logical consistency of the representation
        bool invariant() const
        {
                return side_invariant() && material_invariant();
        }

        // black and white pieces are mutually exclusive
        bool side_invariant() const
        {
                return bit::is_exclusive(pieces(Side::black), pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return bit::is_subset_of(kings(), pieces());
        }

        // initialize with a set of bitboards
        template<bool Color>
        void init(BitBoard active_pieces, BitBoard passive_pieces, BitBoard kings)
        {
                pieces_[ Color] = active_pieces;
                pieces_[!Color] = passive_pieces;
                kings_ = kings;
        }

        // representation
        BitBoard pieces_[2];    // black and white pieces
        BitBoard kings_;        // kings
};

}       // namespace dctl
