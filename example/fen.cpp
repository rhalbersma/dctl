#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/spirit/home/x3.hpp>
#include <cstdint>
#include <iostream>
#include <vector>

namespace x3 = boost::spirit::x3;

// general utilities

template<class T>
auto attribute_cast = [](auto p) { return x3::rule<struct _, T>{} = x3::as_parser(p); };

namespace local {
    template<class T>
    class mutable_wrapper
    {
        T mutable data;
    public:
        auto value() const { return data; }
        auto&  get() const { return data; }
    };

    template<class Tag, class Attribute>
    auto const define = [] {
        return x3::with<Tag>(mutable_wrapper<Attribute>{});
    };

    template<class Tag>
    auto const assign = [](auto p) {
        auto const sa = [](auto& ctx) {
            x3::get<Tag>(ctx).get() = x3::_attr(ctx);
        };
        return x3::omit[x3::as_parser(p)[sa]];
    };
}

namespace ast {
    struct ALG {};
    struct NUM {};

    template<class Format>
    struct occupance;

    template<>
    struct occupance<ALG>
    {
        char color, piece, file;
        uint16_t rank; // uint8_t is dropped during printing
    };

    template<>
    struct occupance<NUM>
    {
        char color, piece;
        uint16_t square;
    };

    template<class Format>
    struct position
    {
        char side_to_move;
        std::vector<occupance<Format>> piece_placement;
    };
}

BOOST_FUSION_ADAPT_STRUCT(ast::occupance<ast::ALG>, color, piece, file, rank)
BOOST_FUSION_ADAPT_STRUCT(ast::occupance<ast::NUM>, color, piece, square    )
BOOST_FUSION_ADAPT_STRUCT(ast::position<ast::ALG>, side_to_move, piece_placement)
BOOST_FUSION_ADAPT_STRUCT(ast::position<ast::NUM>, side_to_move, piece_placement)

namespace ast {
    template<class Format>
    auto& operator<<(std::ostream& os, std::vector<occupance<Format>> const& v)
    {
        using boost::fusion::operators::operator<<;
        for (auto const& elem : v)
            os << elem << " ";
        return os;
    }
}

namespace parser {
    struct color_tag {};

    auto const side_to_move = x3::char_("BW?") | x3::attr('\0');
    auto const color        = x3::char_("BW");
    auto const piece        = attribute_cast<char>(x3::char_('K') | x3::attr('M'));
    auto const file     = x3::char_;
    auto const rank     = x3::uint8;
    auto const square   = x3::uint16;

    template<class Format>
    auto const push = []{};

    template<>
    auto const push<ast::ALG> = [](auto& ctx) {
        using boost::fusion::at_c;
        auto const color = x3::get<color_tag>(ctx).value();
        auto const piece = at_c<0>(x3::_attr(ctx));
        auto const file  = at_c<1>(x3::_attr(ctx));
        auto const rank  = at_c<2>(x3::_attr(ctx));
        x3::_val(ctx).push_back({color, piece, file, rank});
    };

    template<>
    auto const push<ast::NUM> = [](auto& ctx) {
        using boost::fusion::at_c;
        auto const color = x3::get<color_tag>(ctx).value();
        auto const piece = at_c<0>(x3::_attr(ctx));
        auto const first = at_c<1>(x3::_attr(ctx));
        auto const last  = at_c<2>(x3::_attr(ctx)).get_value_or(first);
        for (auto square = first; square <= last; ++square) {
            x3::_val(ctx).push_back({color, piece, square});
        }
    };

    template<class Format>
    auto piece_list = 0;

    template<>
    auto const piece_list<ast::ALG> = piece >> file >> rank;

    template<>
    auto const piece_list<ast::NUM> = piece >> square >> -('-' >> square);

//#define SINGLE_TEMPLATE
#ifdef SINGLE_TEMPLATE
    template<class Format>
    auto const position = attribute_cast<ast::position<Format>>
    (
        side_to_move >> x3::repeat(0,2)
        [
            attribute_cast<std::vector<ast::occupance<Format>>>
            (
                ':' >> -local::define<color_tag, char>()
                [
                    local::assign<color_tag>(color) >> -
                    (
                        (piece_list<Format>)[push<Format>] % ','
                    )
                ]
            )
        ] >> -x3::lit('.')
    );
#else
    template<class Format>
    auto const position = []{};

    template<>
    auto const position<ast::ALG> = attribute_cast<ast::position<ast::ALG>>
    (
        side_to_move >> x3::repeat(0,2)
        [
            attribute_cast<std::vector<ast::occupance<ast::ALG>>>
            (
                ':' >> -local::define<color_tag, char>()
                [
                    local::assign<color_tag>(color) >> -
                    (
                        (piece_list<ast::ALG>)[push<ast::ALG>] % ','
                    )
                ]
            )
        ] >> -x3::lit('.')
    );

    template<>
    auto const position<ast::NUM> = attribute_cast<ast::position<ast::NUM>>
    (
        side_to_move >> x3::repeat(0,2)
        [
            attribute_cast<std::vector<ast::occupance<ast::NUM>>>
            (
                ':' >> -local::define<color_tag, char>()
                [
                    local::assign<color_tag>(color) >> -
                    (
                        (piece_list<ast::NUM>)[push<ast::NUM>] % ','
                    )
                ]
            )
        ] >> -x3::lit('.')
    );
#endif
}

template<class Phrase, class Grammar, class Skipper, class AST>
auto test(Phrase const& phrase, Grammar const& grammar, Skipper const& skipper, AST& data)
{
        auto first = phrase.begin();
        auto last = phrase.end();

        using boost::fusion::operators::operator<<;

        auto const ok = phrase_parse(first, last, grammar, skipper, data);
        if (ok) {
            std::cout << "OK! Parsed: " << data << "\n";
        } else {
            std::cout << "Parse failed:\n";
            std::cout << "\t on input: " << phrase << "\n";
        }
        if (first != last)
            std::cout << "\t Remaining unparsed: '" << std::string(first, last) << '\n';
}

int main() {
    std::string numeric_tests[] =
    {
        "B:W18,24,27,28,K10,K15:B12,16,20,K22,K25,K29",
        "B:W18,19,21,23,24,26,29,30,31,32:B1,2,3,4,6,7,9,10,11,12",
        "W:B01-20:W31-50",  // initial position, should map to W:BM1,...,BM20:WM31,...,WM50
        "W:B:W",            // empty board
        "W:B1:W",           // only black pieces
        "W:B:W50",          // only white pieces
    };

    for (auto const& t : numeric_tests) {
        ast::position<ast::NUM> data;
        test(t, parser::position<ast::NUM>, x3::space, data);
    }

    std::string algebraic_tests[] =
    {
        "W:Wa1,c1,e1,g1,b2,d2,f2,h2,a3,c3,e3,g3:Bb8,d8,f8,h8,a7,c7,e7,g7,b6,d6,f6,h6",
        "W:BKa1,Ka3:WKb8,Kd8",
    };

    for (auto const& t : algebraic_tests) {
        ast::position<ast::ALG> data;
        test(t, parser::position<ast::ALG>, x3::space, data);
    }
}
