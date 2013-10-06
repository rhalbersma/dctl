#pragma once
#include <cassert>                      // assert
#include <type_traits>                  // is_base_of
#include <boost/operators.hpp>          // equality_comparable1
#include <dctl/bit/bit.hpp>             // is_subset
#include <dctl/node/i_pieces.hpp>
#include <dctl/node/side.hpp>
#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/bitboard.hpp>        // BitBoard

namespace dctl {

template<class T>
struct Material_
        // http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm#chaining
        // use base class chaining to ensure Empty Base Optimization
:       boost::equality_comparable1< Material_<T>       // == !=
,       IPieces< Material_, T >
        >
{
        friend class IPieces< ::dctl::Material_, T >;

public:
        enum {
                none = 0,
                pawn = 1,
                king = pawn << 1,
                both = pawn ^ king
        };

        // structors

        // default constructor
        Material_() = default;

        // zero initialize
        explicit Material_(T /* MUST be zero */)
        {
                init<Side::black>(0, 0, 0);
                assert(invariant());
        }

        // initialize with a set of bitboards
        Material_(T black_pieces, T white_pieces, T kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                assert(invariant());
        }

        // modifiers

        // xor-assign the set bits of another piece set
        template<template<class> class U>
        Material_& operator^=(U<T> const& m)
        {
                static_assert(std::is_base_of< IPieces<U, T>, U<T> >::value, "");
                pieces_[Side::black] ^= m.pieces(Side::black);
                pieces_[Side::white] ^= m.pieces(Side::white);
                kings_ ^= m.kings();
                assert(invariant());
                return *this;
        }

        // predicates

        // operator!= provided by boost::equality_comparable1
        friend bool operator==(Material_ const& lhs, Material_ const& rhs)
        {
                return (
                        (lhs.pieces(Side::black) == rhs.pieces(Side::black)) &&
                        (lhs.pieces(Side::white) == rhs.pieces(Side::white)) &&
                                    (lhs.kings() == rhs.kings())
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

        // black or white pawns
        auto do_pawns(bool color) const
        {
                return do_pieces(color) & ~do_kings();
        }

        // black or white kings
        auto do_kings(bool color) const
        {
                return do_pieces(color) & do_kings();
        }

        // black or white pieces
        auto do_pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        auto do_pawns() const
        {
                return do_pieces() & ~do_kings();
        }

        // black and white kings
        auto do_kings() const
        {
                return kings_;
        }

        // black and white pieces
        auto do_pieces() const
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
                return bit::raw_set_exclusive(this->pieces(Side::black), this->pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return bit::raw_set_includes(this->pieces(), this->kings());
        }

        // representation

        T pieces_[2];   // black and white pieces
        T kings_;       // kings
};

using Material = Material_<BitBoard>;

}       // namespace dctl
