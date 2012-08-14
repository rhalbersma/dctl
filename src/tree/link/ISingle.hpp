#pragma once
#include "../../utility/enable_crtp.hpp"

namespace dctl {
namespace tree {
namespace link {

struct single_tag {};

template<template<typename, typename> class Node, typename T>
struct ISingle
:
        private enable_crtp< Node<single_tag, T> >
{
public:
        // typedefs

        typedef Node<single_tag, T>* node_ptr;

        // modifiers

        void attach(node_ptr other)
        {
                self()->do_attach(other);
        }

        void detach()
        {
                self()->do_detach();
        }

        // queries

        node_ptr next() const
        {
                return self()->do_next();
        }

protected:
        // disable deletion of Derived* through Base*
        // enable deletion of Base* through Derived*
        ~ISingle()
        {
                // no-op
        }
};

}       // namespace link
}       // namespace tree
}       // namespace dctl
