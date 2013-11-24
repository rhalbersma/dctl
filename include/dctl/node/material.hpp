#pragma once
#include <cassert>                      // assert
#include <type_traits>                  // is_base_of
#include <boost/operators.hpp>          // equality_comparable1
#include <dctl/node/side.hpp>

#include <cstdint>
#include <dctl/bit/algorithm.hpp>
#include <dctl/bit/bit_set.hpp>

namespace dctl {

template<class T>
struct Material_
        // http://www.boost.org/doc/libs/1_51_0/libs/utility/operators.htm#chaining
:
        boost::equality_comparable1< Material_<T> >     // == !=
{
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
                init<Side::black>(T{}, T{}, T{});
                assert(invariant());
        }

        // initialize with a set of bitboards
        Material_(T black_pieces, T white_pieces, T kings)
        {
                init<Side::black>(black_pieces, white_pieces, kings);
                assert(invariant());
        }

        // modifiers

        template<class U>
        Material_& make(U const& m)
        {
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

        // queries

        // black or white pawns
        auto pawns(bool color) const
        {
                return pieces(color) & ~kings();
        }

        // black or white kings
        auto kings(bool color) const
        {
                return pieces(color) & kings();
        }

        // black or white pieces
        auto pieces(bool color) const
        {
                return pieces_[color];
        }

        // black and white pawns
        auto pawns() const
        {
                return pieces() & ~kings();
        }

        // black and white kings
        auto kings() const
        {
                return kings_;
        }

        // black and white pieces
        auto pieces() const
        {
                return pieces(Side::black) ^ pieces(Side::white);
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

        // logical consistency of the representation
        bool invariant() const
        {
                return side_invariant() && material_invariant();
        }

        // black and white pieces are mutually exclusive
        bool side_invariant() const
        {
                return bit::set_exclusive(pieces(Side::black), pieces(Side::white));
        }

        // kings are a subset of pieces
        bool material_invariant() const
        {
                return bit::set_includes(pieces(), kings());
        }

        // representation

        T pieces_[2];   // black and white pieces
        T kings_;       // kings
};

using Material = Material_< bit::bit_set<int, uint64_t, 1> >;

}       // namespace dctl
