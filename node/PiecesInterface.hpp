#pragma once
#include "../utility/IntegerTypes.hpp"

namespace dctl {

template<typename Impl>
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

protected:
        // disable deletion of Derived* through Base* 
        // enable deletion of Base* through Derived*
        ~PiecesInterface()
        {
                // no-op
        }

private:
        // cast a base reference to a derived reference (i.e. "down" the class hierarchy)
        const Impl& down_cast(const PiecesInterface<Impl>& other) const
        {
                return static_cast<const Impl&>(other);
        }

        // cast the this pointer to a derived reference
        const Impl& self() const
        {
                return down_cast(*this);
        }
};

}       // namespace dctl
