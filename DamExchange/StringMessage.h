#pragma once
#include <string>

namespace DamExchangeProtocol {

#include <cstdio>
#include <cstdlib>
#include <cstring>

class StringMessage
{
public:
        enum { HEADER_LENGTH = 1 };
        enum { MAX_BODY_LENGTH = 127 };

        StringMessage(void);
        explicit StringMessage(const std::string&);
        StringMessage(const std::string&, const std::string&);

        std::string data(void) const;
        char* data(void);
        size_t length(void) const;

        std::string header(void) const;
        std::string body(void) const;
        char* body(void);
        size_t body_length(void) const;
        void body_length(size_t length);

        bool decode_header(void);
        void encode_header(void);

private:
        char data_[HEADER_LENGTH + MAX_BODY_LENGTH];
        size_t body_length_;
};

} // namespace DamExchangeProtocol
