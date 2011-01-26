#pragma once
#include "AbstractMessage.h"
#include "StringMessage.h"
#include <string>
#include <vector>
#include <memory>

namespace DamExchangeProtocol {

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

        static std::shared_ptr<AbstractMessage> create(const StringMessage&);
        static bool pre_condition(const StringMessage&);

        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        size_t seconds_;
        size_t from_sq_;
        size_t dest_sq_;
        size_t num_captured_;
        std::vector<size_t> captured_pieces_;       
};

}       // namespace DamExchangeProtocol
