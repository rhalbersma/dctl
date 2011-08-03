#pragma once

namespace dctl {

class Iota
{
public:
        Iota(int = 0);
        int operator()();

private:
        int current_;
};

}       // namespace dctl
