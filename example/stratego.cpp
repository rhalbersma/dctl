#include <dctl/egdb/index.hpp>
#include <xstd/int_set.hpp>
#include <algorithm>
#include <iostream>

using board = xstd::int_set<40>;
constexpr auto W = 10;
constexpr auto H = 4;
constexpr auto B = W * H;

auto square(int r, int c)
{
        return r * W + c;
}

auto& operator<<(std::ostream& ostr, board const& b)
{
        for (auto r = 0; r < H; ++r) {
                ostr << (4 - r) << ' ';
                for (auto c = 0; c < W; ++c) {
                        ostr << (b.contains(square(r, c)) ? "B " : ". ");
                }
                ostr << '\n';
        }
        ostr << "  ";
        auto col = 'a';
        for (auto c = 0; c < W; ++c) {
                ostr << col++  << ' ';
        }
        return ostr << '\n';
}

int main()
{
        std::cout << "Hello, Stratego brothers!\n\n";

        constexpr auto b23 = board{0, 1, 10, 11, 20, 21};
        constexpr auto b32 = board{0, 1, 2, 10, 11, 12};
        constexpr auto b61 = board{0, 1, 2, 3, 4, 5 };

        std::vector<board> p23;
        for (auto r = 0; r < H + 1 - 3; ++r) {
                for (auto c = 0; c < W + 1 - 2; ++c) {
                        p23.push_back(b23 << square(r, c));
                }
        }
        std::vector<board> p32;
        for (auto r = 0; r < H + 1 - 2; ++r) {
                for (auto c = 0; c < W + 1 - 3; ++c) {
                        p32.push_back(b32 << square(r, c));
                }
        }
        std::vector<board> p61;
        for (auto r = 0; r < H + 1 - 1; ++r) {
                for (auto c = 0; c < W + 1 - 6; ++c) {
                        p61.push_back(b61 << square(r, c));
                }
        }

        auto maxroelofs = p23;
        maxroelofs.insert(maxroelofs.end(), p32.begin(), p32.end());
        auto napoleon = maxroelofs;
        napoleon.insert(napoleon.end(), p61.begin(), p61.end());

        auto id = [](auto x) { return x; };
        auto const& patterns = napoleon;

        for (auto n = 0; n < 8; ++n) {
                for (auto idx = 0LL; idx < dctl::egdb::choose(B, n); ++idx) {
                        auto const b = dctl::egdb::colex_unrank_combination<board>(idx, B, n, id);
                        auto result = std::none_of(patterns.begin(), patterns.end(), [&](auto const& p){
                                return (b & p).empty();
                        });
                        if (result) {
                                std::cout << b << '\n';
                        }
                }
        }
}
