

#include "TreeSearch/Test.h"
#include "TreeWalk/Test.h"
#include <iostream>
#include <iomanip>

#include "DamExchange/Message/Test.h"
#include "DamExchange/Connection/Client.h"

int main(void)
{     
        DamExchangeProtocol::Message::Test::Mesander_examples();

        DamExchangeProtocol::Connection::Client dxp_connection;

        // only one of these next two statements can be active!!
        //dxp_connection.connect("127.0.0.1", "27531");
        dxp_connection.accept("27531");

        std::string line;
        while (std::getline(std::cin, line))
        {                
                dxp_connection.write(line);
        }
        dxp_connection.close();

        
        //TreeWalk::Test::International();
        //TreeWalk::Test::Killer();
        //TreeWalk::Test::Rectangular();
        //TreeWalk::Test::ChessBoardVariants();
        //TreeWalk::Test::ItalianRuleBook();
        
        std::cout << "End of program." << std::endl;
        for (;;);
}

/*
TODO:
-split Capture into Capture + MoveList, change Move into MoveList
-test DXP Layer 1 (using Boost sockets)
-implement algebraic notation
-implement missing square layouts
-implement test harness for hash function
-search...
-eval...
*/
/*
#include <cstdint>
#include <iostream>
#include <iomanip>
#include "Utilities/DeBruijn.h"

template<typename> struct log2_sizeof;
template<> struct log2_sizeof<uint8_t>           { enum { value = 3 }; };
template<> struct log2_sizeof<uint16_t>          { enum { value = 4 }; };
template<> struct log2_sizeof<uint32_t>          { enum { value = 5 }; };
template<> struct log2_sizeof<uint64_t>          { enum { value = 6 }; };

template<typename T>
size_t index_DeBruijn(T b)
{
        static const size_t N = log2_sizeof<T>::value;
        b *= static_cast<T>(DeBruijn<N>::SEQUENCE);
        b >>= DeBruijn<N>::SHIFT;
        return DeBruijn<N>::TABLE[b];
}

int main(void)
{
        DeBruijn<1>::generate_table();
        DeBruijn<2>::generate_table();
        DeBruijn<3>::generate_table();
        DeBruijn<4>::generate_table();
        DeBruijn<5>::generate_table();
        DeBruijn<6>::generate_table();

        uint8_t i8 = uint8_t(1) << 7;
        uint16_t i16 = uint16_t(1) << 15;
        uint32_t i32 = uint32_t(1) << 31;
        uint64_t i64 = uint64_t(1) << 63;

        std::cout << index_DeBruijn(i8) << std::endl;
        std::cout << index_DeBruijn(i16) << std::endl;
        std::cout << index_DeBruijn(i32) << std::endl;
        std::cout << index_DeBruijn(i64) << std::endl;

        //std::cout << "End of program." << std::endl;
        for (;;);
}
*/