#pragma once
#include "../../bit/Bit.hpp"
#include "../../utility/IntegerTypes.hpp"

namespace dctl {
namespace hash {
namespace zobrist {

template
<
        typename Index = uint64_t
>
struct Random
{
        // xor with random numbers matching a bitboard's set 1-bits
        static Index xor_rand(BitBoard b, Index const* random)
        {
                Index hash = 0;
                for (; b; bit::clear_first(b))
                        hash ^= random[bit::find_first(b)];
                return hash;
        }

        // xor with random numbers conditional on a ply count
        static Index xor_rand(PlyCount s, Index const* random)
        {
                return s? random[s - 1] : 0;
        }

        // xor with a random number conditional on a boolean
        static Index xor_rand(bool to_move, Index random)
        {
                return to_move? random : 0;
        }

        static Index const PIECES[2][64];
        static Index const KINGS[64];
        static Index const SIDE;
        static Index const RESTRICTED_KING[2][64];
        static Index const RESTRICTED_MOVES[2][8];
};

template<typename Index>
Index const Random<Index>::PIECES[2][64] = {
        {
                0x9904662fb3c24ae1, 0xc59adcabb4a95f90, 0x9dbe36bc60dda6e9, 0xb1766ba9896d953c,
                0x772183cf804c8124, 0xe881b24726834db7, 0x7a9bce6cd689074d, 0x8c8879aee145a6a6,
                0xfba07e96bb3ec509, 0x8764a787f72e9ad4, 0xbd7f9fe5bfface45, 0xb442698375c4830a,
                0xc17b75ab83990120, 0xa46a08bba5a05878, 0x6b9faf848d24df32, 0x6753de5f6fa48ddc,
                0xd629f78526f94944, 0x026cc7341b35e6c4, 0x0fd6f80c961d95a1, 0x27dec208acc8c09d,
                0x1f316487f9ed58b0, 0xa6d32240bddcd772, 0xde093a4b2540187e, 0x5d132f3549f4c080,
                0x267d48f264665753, 0xfb8cef3587dfaf14, 0x608fd08b6a6e368e, 0x6f307d7be32bac49,
                0x15fffb1a5447ba61, 0xdfba2987af4ec89c, 0x6c2e5adc9973401d, 0x00709fd3e4a50677,
                0x231afafa281cd42c, 0x3d7963f247095f1e, 0x3f0e8927958611f4, 0x5d5a96a7e66a5fa4,
                0x924fc00d43e7e732, 0x235e83b3236781d3, 0x0745b8122be9c58f, 0x56fcc6766c8ea52a,
                0x53b8127de215d078, 0x1810fed12c87765f, 0x056005b53fb0ba8d, 0xe18f7401460ab807,
                0xb0c67ca0928f9d18, 0x06924676a8324d54, 0xe7763321689f6d69, 0x59ff1711a4353004,
                0x6bce71c3b9ec9f16, 0xf9acf65a256a2cf7, 0xa14957b4272bcf68, 0x6cdf44c5a4ddef11,
                0xd9d5d74902c45579, 0xe50a5236d3929643, 0x5de79b3d2fa293d3, 0x81450273a905e8f5,
                0x572c0d16531ebb97, 0x72714a566f3b282d, 0x0f585bf26e66a668, 0x4209221ab23cc325,
                0x8f4c2250380a53b1, 0xb1ad50294e88a528, 0x18d195358754bf0b, 0x77f2f9632097b0f1
        },
        {
                0x5883b366994a80c1, 0xb19c33f0373f3ee9, 0x87f34c29f24346c2, 0x5f4a942fcb4d2fe7,
                0x9be47e74672a8390, 0x80e51474486cec6c, 0x198cd717b6a701e1, 0x0b5393bf8dec8871,
                0xf9fc4ae954712f0c, 0xbcd699d1e1b2ce38, 0xae68c4a26854b382, 0x922f5b5d843c36c2,
                0x34d631898e6fa7da, 0x9ceda44f923164e0, 0x6ca257cd275fae39, 0x63a7fba24eaf1ff1,
                0xdfc1c1b8c5364328, 0x1183c848390e9dcb, 0x22007dcfc5208607, 0x4561fd451982b765,
                0xbd65111875e4d8c5, 0x74258023a1a37a36, 0x83db92bcd054578d, 0xe6f7dc7c8075f871,
                0xfc941274091d9c75, 0x60118d5f8543d478, 0x3109b9f42a5f5882, 0xe87b75f089235841,
                0xbc7180feb798527f, 0x015e11a9fcaa0786, 0x2252bf6aa7ada568, 0x35e176398dfe13f5,
                0xa857dcdf46cff389, 0xa1483906cafefcb8, 0xfded06bd9d39007b, 0x41d32cf353e9ff0d,
                0x0408860e8fda13a6, 0x6e2a7e0ccc85e3cb, 0x1a821d179d2675ec, 0x3269945ce4733605,
                0x48a63283a25520ad, 0xf59aa12c32e51927, 0xf63b12e7448a194d, 0x4a4f2e89e6acd942,
                0xcaf38cb0757c4bcd, 0xcf30fc081911fe64, 0x8e54d261dc4ad04c, 0x00cb492350a7342d,
                0x9b5cf74f7958b260, 0xee7629e018d6c908, 0x1bbd37d9ef20dea5, 0x5333e8b92b8c749f,
                0x584843803a1319fe, 0xca7a2e086c020b93, 0xf13660e3d6b9a753, 0x02486ca28457d67e,
                0xa32df6356767d3d3, 0x1192b173832d7cbe, 0x5f84f74aa1fb5707, 0xd5f42c6f8332e7a6,
                0x40f929f032349b32, 0x47c1204b1c26c101, 0xfc22ecd596636bdb, 0xd9f91fe9836c480b
        }
};

template<typename Index>
Index const Random<Index>::KINGS[64] = {
        0x863029ce1230836b, 0x8642409881fcba54, 0x0efcecdc6e8bce48, 0x8e01b2a4a6148a1d,
        0x676532e665b948db, 0x4ac38afe80a7a237, 0x83b6b8ac17caef57, 0x5b9bf4203936176d,
        0xde6e98f67dc59943, 0x5cc23e823652f1f6, 0x92f7b3bdd0f5a015, 0x8c9c05786db03d8f,
        0xece79b5ec7fa275b, 0x279158628c47dd27, 0xc03ef4b96c43964a, 0xb46655a6f5b0c740,
        0x4c78c770e8d89da7, 0x0f89490401776275, 0xf0c0604bd64f526a, 0xf6a20d5405cdc5c8,
        0x1568750a7814448a, 0x60dd2ee790a3b6a1, 0x17c902cc64365acd, 0x4dd3db3792c0eea3,
        0xcbf31a8430ffef9a, 0x3e99b0b3dc2b8dc7, 0x0d2b2dafcade801c, 0x7c60620a05bfb15d,
        0x9cebe4ee49248634, 0xe04c1f4f297286de, 0x2a330dd081591f0d, 0xba76d905017669bb,
        0x8a25f19cfef13b51, 0xe4cfb90755ec3d84, 0x31a3ff80705f1350, 0xbb62dff5b1a1d221,
        0x3f233605528c4898, 0xb8c596cb8fc5c5f9, 0xef3b956f30f84fca, 0xecc7b7db7f4a1a31,
        0x2e8e92ebfac6c0b5, 0x47321c757c2f176a, 0x033f97603c3b5002, 0xba42e11ae39b45b1,
        0x10fb7fdf482d4cea, 0x22c3d5209546a871, 0x86918dae0e2f56d9, 0xe7f5f03eb35fb7b0,
        0x456e8c07483ae9e5, 0xce42149629a341da, 0x2bcb7ea05dd02e7f, 0xce797a475d1b0de9,
        0x9c2e81409aaaf7d2, 0xc2a346c2c77bcba9, 0x69e472902c2ddda3, 0x3ab7298d73c43068,
        0xdc5c268a8aeab1b2, 0x294ba642bd72985a, 0xc4dd23ddccae85eb, 0x5c2d3c31401d396c,
        0x3cd30ec35fabdfeb, 0x78f342f7fb027268, 0x20f44fb568735fa4, 0x64179d1df2af998d
};

template<typename Index>
Index const Random<Index>::SIDE = 0x461aea9b6bcff19a;

template<typename Index>
Index const Random<Index>::RESTRICTED_KING[2][64] = {
        {
                0x30d1f0b5b2955a21, 0x4bb531b53a827011, 0x14e30ca35ed706bb, 0x3003989830651c20,
                0xf0ebb5758217ac45, 0x3cc52df5fed45159, 0xc7a1bbfa85130ced, 0x5249dd6013c22fd9,
                0xb0bf1a0dcf5c72ed, 0x544f46a80eae6c4e, 0xae33fb0866fe053c, 0x62f0ab639cc097b0,
                0xd9f7f17bba58c2e4, 0x5ffd3f04b783eb8b, 0x65fd89198af17ca3, 0x3b7720fd29486806,
                0x2d1723fd2e023002, 0xdea59c78020f6301, 0x401f0750dac8140b, 0x2806e7c5b0497408,
                0x308398ad4e9bc73e, 0x7da1ca9ca27dfef2, 0x6a598749b404af7e, 0x608f51f619768f55,
                0x7e624672bc2b9002, 0x6e4d8e420e37513f, 0x09fc0d8e810e6e9b, 0xf4474957eba047dd,
                0xa9b739391b0de8c0, 0x622c3087414900e5, 0x85579cde1d95e24a, 0x7a1860c62bacfc0a,
                0xeb4d4882b4a152c4, 0x1437db03dd71dfc3, 0x103b6f3fe81833b2, 0xa071293e2632f122,
                0x1ee78c41621e814a, 0x3dd7cdff6cc6aa9e, 0x97fa11e8427f3069, 0x4eff5b813db3cded,
                0xf1392d06538a3fd1, 0xfdff09bcb2047768, 0x1375a8f023e2efec, 0x24d29744b7761d94,
                0x97261c7da4c183af, 0x30f2b3d0507195bd, 0xd3b42fd63d6d064f, 0x156708f3a0febeca,
                0x5ecb933bd2b2bce5, 0x9f38918c356f65af, 0xc97c5bd7945b7836, 0xbf17d700082b632f,
                0x72d080da82aa3a36, 0xf22d80768d9e68c5, 0xac797512531ee405, 0x3d7947c39feb82f2,
                0x107e8b662dc7b474, 0x84c445d3efac113f, 0x4868eb7bf3a11454, 0xe21be0ef8b9ce8b6,
                0x69218b19e28e551b, 0x4eeb9e3f86bb939d, 0xbccddca4c4a46fa7, 0x4636718fab0139b8
        },
        {
                0x4f30ff6dae984c22, 0xd21c8655a66ebe61, 0xb3adcf4f6c46795c, 0x7fb88799e0de1e2f,
                0x27bfea726266490f, 0x3592056a52921c11, 0xebd5b9d2e9a0d7e5, 0x2c45020fee316df5,
                0x80b5cf7f4154eb49, 0xb8a6335464656380, 0xcb25d851778d1439, 0x5c9540af9c0e0565,
                0xe44ee22f4fabfdb3, 0x39d92245d7857a4e, 0x656647bd92878686, 0x02c262348e16a684,
                0x7a64c2ad90d11178, 0x890045b468770daf, 0x3c242aae83aca429, 0xedf8d229b7950a65,
                0xfffe9451e79b4c21, 0xb31bbf52ced1056a, 0x94150d04b2e022e0, 0x461237480d3706c7,
                0x25a9eb8f6ac27cee, 0x0e550b1a7d065522, 0x04848b5fe20c253f, 0x8198036a6d65ec01,
                0xfd1b202f5f724d64, 0x84a67362efcd6fd9, 0x3d48136393836461, 0x78ae960c9941ff20,
                0xc695e0e0ebfea123, 0x23a8860c772e61b3, 0x87bb41cc3d848ae1, 0x1f66925d783bb450,
                0xa69db512b4b1f8fb, 0xd00933b8432a53f9, 0x2f4593538cdb350a, 0x79f859de404f8d79,
                0xc46e0a2334c06d3f, 0x0329b211de057f5f, 0x45e54d6698a1ca4b, 0x95660c96bcddc12b,
                0x49bb98db055a935a, 0xd165ca271e325087, 0x8144f3aa092423ee, 0x87067fd20e240dbd,
                0xe43bc03c64e162a1, 0x3f81cbd279dac2c7, 0x04d78f4f93e1350b, 0x2d772c739b5bdcb1,
                0x1176629db0326c69, 0x35c10f33460ab528, 0xc57d7d3a4ead27b8, 0xf576e5d5516d1a5c,
                0x355d4218201f7b59, 0x5724bd370b793daf, 0xdb3007f4bbf2a383, 0x7d28ba3a685065c4,
                0xc42ce943fcfd6e02, 0xbb51e832baf23ee1, 0x9d3722745216b122, 0x683dcdcf0901fccb
        }
};

template<typename Index>
Index const Random<Index>::RESTRICTED_MOVES[2][8] = {
        {
                0xd0155d45da58e0ae, 0x51a0ad8a6b634382, 0xfd6508b01ff7ef6a, 0x2f88513a4e1f888b,
                0x1329e62c4cc57a77, 0x7bd2096bab82649f, 0xdedadb0490930d7f, 0xce75c4bf3922ae01
        },
        {
                0x461aea9b6bcff19a, 0x76eeb59691183dcd, 0x0cd539696fc3be4b, 0x48f1c8ebd63806ee,
                0x3c34cbca92129b0a, 0xb0cd7c360fecb3b0, 0xbe011375b2271932, 0x1c302ddde8bbe6fb
        }
};

// explicit instantation
template struct Random<>;

}       // namespace zobrist
}       // namespace hash
}       // namespace dctl

/*
Index rand64()
{
        Index key = rand();

        for (auto i = 0; i < 4; ++i) {
                key <<= 15;
                key ^= rand();
        }

        return  key;
}

void generate_Zobrist()
{
        for (auto i = 0; i < 64; ++i) {
                std::cout << "0x" << std::hex << std::right << std::setfill('0') << std::setw(16);
                std::cout << rand64();
                if (i != 63)
                        std::cout << ",";
                if (3 == (i % 4))
                        std::cout << "\n";
                else
                        std::cout << " ";
        }
        std::cout << "\n";
}
*/
