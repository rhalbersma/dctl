#pragma once
#include <string>

class DXP_AbstractMessage
{
public:
        virtual char header(void) const = 0;
        virtual std::string message(void) const = 0;

protected:
        bool invariant(char) const;
};
