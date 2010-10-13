#pragma once
#include <functional>

template<typename Key, typename Index>
struct JenkinsHash: public std::unary_function<Key, Index>
{
        Index operator()(const Key&) const;
};

// include template definitions inside header because "export" keyword is not supported by Visual C++
#include "JenkinsHash.hpp"
