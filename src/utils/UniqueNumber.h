#pragma once

namespace dctl {

class UniqueNumber
{
public:
        UniqueNumber();
        int operator()();

private:
        int current_;
};

}       // namespace dctl
