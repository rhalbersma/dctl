#pragma once
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include <vector>       // std::vector
#include "MessageInterface.h"

namespace dctl {
namespace dxp {

class Move: public MessageInterface
{
public:
        // views
        int seconds() const;
        int from_sq() const;
        int dest_sq() const;
        int num_captured() const;
        const std::vector<int>& captured_pieces() const;
        static std::string str(int, int, int, int, const std::vector<int>&);

private:
        // private constructor to enforce factory creation
        explicit Move(const std::string&);

        // implementation
        virtual std::string header() const;
        virtual std::string body() const;
        static std::string body(int, int, int, int, const std::vector<int>&);

        static std::unique_ptr<MessageInterface> create(const std::string&);
        static const std::string HEADER;
        static const bool REGISTERED;

        // representation
        int seconds_;
        int from_sq_;
        int dest_sq_;
        int num_captured_;
        std::vector<int> captured_pieces_;       
};

}       // namespace dxp
}       // namespace dctl
