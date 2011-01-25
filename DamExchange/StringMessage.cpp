#include "StringMessage.h"
#include <cassert>

namespace DXP = DamExchangeProtocol;

DXP::StringMessage::StringMessage(void)
: 
        body_length_(0)
{
}

DXP::StringMessage::StringMessage(const std::string& s)
: 
        body_length_(s.size() - HEADER_LENGTH)
{
        std::memcpy(data_, s.c_str(), HEADER_LENGTH + body_length_ + 1);
}

std::string DXP::StringMessage::data(void) const
{
        return std::string(data_, length());
}

char* DXP::StringMessage::data(void)
{
        return data_;
}

size_t DXP::StringMessage::length(void) const
{
        return HEADER_LENGTH + body_length_;
}

std::string DXP::StringMessage::header(void) const
{
        return std::string(data_, HEADER_LENGTH);
}

std::string DXP::StringMessage::body(void) const
{
        return std::string(data_ + HEADER_LENGTH, body_length());
}

char* DXP::StringMessage::body(void)
{
        return data_ + HEADER_LENGTH;
}

size_t DXP::StringMessage::body_length(void) const
{
        return body_length_;
}

void DXP::StringMessage::body_length(size_t length)
{
        body_length_ = length;
        if (body_length_ > MAX_BODY_LENGTH)
                body_length_ = MAX_BODY_LENGTH;
}

bool DXP::StringMessage::decode_header(void)
{
        char header[HEADER_LENGTH + 1] = "";
        strncat_s(header, data_, HEADER_LENGTH);
        body_length_ = std::atoi(header);
        if (body_length_ > MAX_BODY_LENGTH) {
                body_length_ = 0;
                return false;
        }
        return true;
}

void DXP::StringMessage::encode_header(void)
{
        char header[HEADER_LENGTH + 1] = "";
        sprintf_s(header, "%4d", body_length_);
        std::memcpy(data_, header, HEADER_LENGTH);
}
