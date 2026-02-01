#pragma once

#ifndef __LIB_DEFINE_H
#define __LIB_DEFINE_H

#include <stdint.h>
#include <stddef.h>

#define memset __builtin_memset
#define memcpy __builtin_memcpy
#define memcmp __builtin_memcmp

#define EXTERN_C extern "C"

#define C_PTR( x )   ( ( void*       ) ( x ) )
#define U_PTR( x )   ( ( uintptr_t   ) ( x ) )
#define U_PTR32( x ) ( ( uint32_t    ) ( x ) )
#define U_PTR64( x ) ( ( uint64_t    ) ( x ) )
#define A_PTR( x )   ( ( char*       ) ( x ) )
#define W_PTR( x )   ( ( wchar_t*    ) ( x ) )

#define C_DEF( x )   ( * ( void**    ) ( x ) )
#define C_DEF08( x ) ( * ( uint8_t*  ) ( x ) )
#define C_DEF16( x ) ( * ( uint16_t* ) ( x ) )
#define C_DEF32( x ) ( * ( uint32_t* ) ( x ) )
#define C_DEF64( x ) ( * ( uint64_t* ) ( x ) )

#define HTON16( x ) (__builtin_bswap16( x ) )
#define HTON32( x ) (__builtin_bswap32( x ) )

#define PAGE_ALIGN( x ) (((uintptr_t) (x) ) + ((0x1000 - (((uintptr_t) (x) ) & (0x1000 - 1))) % 0x1000))

#define SEC( x )    __attribute__((section( #x )))
#define NOINLINE    __attribute__((noinline))
#define INLINE      __attribute__((always_inline))

#define VA_COUNT(...) VA_COUNT_IMPL(__VA_ARGS__,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)
#define VA_COUNT_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,N,...) ( N )

#endif // __LIB_DEFINE_H
