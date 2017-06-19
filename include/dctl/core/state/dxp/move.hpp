#pragma once
#include <iomanip>      // setfill, setw
#include <sstream>      // stringstream
#include <string>       // stoi, string
#include <vector>       // vector

namespace dctl::core {
namespace dxp {

/*

        The format and semantics of Action are defined at:
        http://www.mesander.nl/damexchange/emove.htm

*/

class move
{
        inline static auto const s_header = "M";
        int m_seconds;
        int m_from_sq;
        int m_dest_sq;
        std::vector<int> m_captured_pieces;
public:
        explicit move(std::string const& message)
        :
                m_seconds{std::stoi(message.substr(0, 4))},
                m_from_sq{std::stoi(message.substr(4, 2))},
                m_dest_sq{std::stoi(message.substr(6, 2))}
        {
                for (auto i = 0, n = std::stoi(message.substr(8, 2)); i < n; ++i) {
                        auto const index = static_cast<std::size_t>(10 + 2 * i);
                        m_captured_pieces.push_back(std::stoi(message.substr(index, 2)));
                }
        }

        static auto header() noexcept
        {
                return s_header;
        }

        auto seconds() const noexcept
        {
                return m_seconds;
        }

        auto from_sq() const noexcept
        {
                return m_from_sq;
        }

        auto dest_sq() const noexcept
        {
                return m_dest_sq;
        }

        auto num_captured_pieces() const noexcept
        {
                return static_cast<int>(m_captured_pieces.size());
        }

        auto captured_pieces() const
        {
                return m_captured_pieces;
        }

        auto str() const
        {
                std::stringstream sstr;
                sstr << header();
                sstr << std::setw(4) << std::setfill('0') << seconds();
                sstr << std::setw(2) << std::setfill('0') << from_sq();
                sstr << std::setw(2) << std::setfill('0') << dest_sq();
                sstr << std::setw(2) << std::setfill('0') << num_captured_pieces();
                for (auto const c : captured_pieces()) {
                        sstr << std::setw(2) << std::setfill('0') << c;
                }
                return sstr.str();
        }
};

}       // namespace dxp
}       // namespace dctl::core
