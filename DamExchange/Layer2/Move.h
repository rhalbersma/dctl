#pragma once
#include "AbstractMessage.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace DamExchange {
namespace Layer2 {

class Move: public AbstractMessage
{
public:
        // constructors
        explicit Move(const std::string&);
        Move(size_t, size_t, size_t, size_t, const std::vector<size_t>&);

        // views
        size_t seconds(void) const;
        size_t from_sq(void) const;
        size_t dest_sq(void) const;
        size_t num_captured(void) const;
        const std::vector<size_t>& captured_pieces(void) const;

private:
        // implementation
        virtual std::string header(void) const;
        virtual std::string body(void) const;

        static std::shared_ptr<AbstractMessage> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t seconds_;
        size_t from_sq_;
        size_t dest_sq_;
        size_t num_captured_;
        std::vector<size_t> captured_pieces_;       
};

}       // namespace Layer2
}       // namespace DamExchange
