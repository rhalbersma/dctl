#pragma once
#include <array>                        // array
#include <boost/operators.hpp>          // equality_comparable
#include "PiecesInterface.hpp"
#include "Move.hpp"
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename T>
struct Material_
:
        // Curiously Recurring Template Pattern (CRTP)
        public PiecesInterface< T, Material_ >,
        private boost::equality_comparable< Material_<T> >
{
        enum Composition {
                none = 0,
                pawn = 1,
                king = pawn << 1,
                both = pawn ^ king
        };

        // structors

        // default constructor
        Material_()
        {
                // no-op
        }

        // zero initialize
        explicit Material_(T /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                BOOST_ASSERT(invariant());
        }

        // initialize with a set of bitboards
        Material_(T black_pieces, T white_pieces, T kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                BOOST_ASSERT(invariant());
        }

        // modifiers

        // xor-assign the set bits of another piece set
        Material_& operator^=(const Move_<T>& m)
        {
                pieces_[Side::black] ^= m.pieces(Side::black);
                pieces_[Side::white] ^= m.pieces(Side::white);
                kings_ ^= m.kings();
                BOOST_ASSERT(invariant());
                return *this;
        }

        // predicates

        bool operator==(const Material_& other) const
        {
                return (
                        (pieces(Side::black) == other.pieces(Side::black)) &&
                        (pieces(Side::white) == other.pieces(Side::white)) &&
                                    (kings() == other.kings())
                );
        }

private:
        // modifiers

        // initialize with a set of bitboards
        template<bool Color>
        void init(T active_pieces, T passive_pieces, T kings)
        {
                pieces_[ Color] = active_pieces;
                pieces_[!Color] = passive_pieces;
                kings_ = kings;
        }

        // queries

        friend struct PiecesInterface< T, ::dctl::Material_ >;

        // black or white pawns
        T do_pawns(bool color) const
        {
                return do_pieces(color) & ~do_kings();
        }

        // black or white kings
        T do_kings(bool color) const
        {
                return do_pieces(color) & do_kings();
        }

        // black or white pieces
        T do_pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        T do_pawns() const
        {
                return do_pieces() & ~do_kings();
        }

        // black and white kings
        T do_kings() const
        {
                return kings_;
        }

        // black and white pieces
        T do_pieces() const
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

        // representation

        std::array<T, 2> pieces_;       // black and white pieces
        T kings_;                       // kings
};

typedef Material_<BitBoard> Material;

}       // namespace dctl
