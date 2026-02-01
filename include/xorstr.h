#pragma once

#ifndef __LIB_XORSTR_H
#define __LIB_XORSTR_H

#include <stdint.h>
#include <stddef.h>

namespace xorstr
{
    constexpr uint32_t fnXorShift32(uint32_t s)
    {
        s ^= s << 13;
        s ^= s >> 17;
        s ^= s << 5;
        return s;
    }

    template<std::size_t N>
    struct CString
    {
        char data[N];
        constexpr CString(const char (&str)[N])
        {
            for (std::size_t i = 0; i < N; i++)
                data[i] = str[i];
        }
    };

    template<std::size_t N, uint32_t Seed>
    struct CEncrypted
    {
        unsigned char data[N];
        constexpr CEncrypted() : data{}
        {
            uint32_t s = Seed;
            for (std::size_t i = 0; i < N; i++)
            {
                s = fnXorShift32(s);
                data[i] = 0;
            }
        }
    };

    template<CString Str, uint32_t Seed>
    struct CEncryptedHolder
    {
        static constexpr auto MakeEncrypted()
        {
            CEncrypted<sizeof(Str.data), Seed> enc;
            uint32_t s = Seed;
            for (std::size_t i = 0; i < sizeof(Str.data); i++)
            {
                s = fnXorShift32(s);
                enc.data[i] = static_cast<unsigned char>(Str.data[i] ^ (s & 0xFF));
            }
            return enc;
        }

        static constexpr auto enc = MakeEncrypted();
    };

    constexpr uint32_t fnMakeSeed(uint32_t line, uint32_t len)
    {
        return ((0x9E3779B1ULL) ^ (0x85EBCA77ULL * len));
    }

	__attribute__((noinline))
    inline void fnDecrypt(const unsigned char* pEnc, char* szOut, std::size_t n, uint32_t seed)
    {
        uint32_t s = seed;
        for (std::size_t i = 0; i < n; i++)
        {
            s = fnXorShift32(s);
            szOut[i] = static_cast<char>(pEnc[i] ^ (s & 0xFF));
        }
    }
}

#define XORSTR(str) \
({ \
    constexpr auto seed = xorstr::fnMakeSeed(0, sizeof(str)); \
    static constexpr auto& enc = xorstr::CEncryptedHolder<str, seed>::enc; \
    char* szOut = (char*)alloca(sizeof(str)); \
    xorstr::fnDecrypt(enc.data, szOut, sizeof(str), seed); \
    (const char*)szOut; \
})

#endif // __LIB_XORSTR_H
