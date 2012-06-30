#pragma once
#include "../utility/enable_crtp.hpp"   // enable_crtp

namespace dctl {

template
<
        template<typename> class Impl,
        typename T
>
class PiecesInterface
:
        // enable static polymorphism
        private enable_crtp< Impl<T> >
{
public:
        // queries

        // black or white pawns
        T pawns(bool color) const
        {
                return self()->do_pawns(color);
        }

        // black or white kings
        T kings(bool color) const
        {
                return self()->do_kings(color);
        }

        // black or white pieces
        T pieces(bool color) const
        {
                return self()->do_pieces(color);
        }

        // black and white pawns
        T pawns() const
        {
                return self()->do_pawns();
        }

        // black and white kings
        T kings() const
        {
                return self()->do_kings();
        }

        // black and white pieces
        T pieces() const
        {
                return self()->do_pieces();
        }

protected:
        // disable deletion of Derived* through Base* 
        // enable deletion of Base* through Derived*
        ~PiecesInterface()
        {
                // no-op
        }
};

}       // namespace dctl
