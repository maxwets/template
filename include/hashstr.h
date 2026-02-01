#pragma once

#ifndef __LIB_HASHSTR_H
#define __LIB_HASHSTR_H

#include <stdint.h>
#include <stddef.h>

namespace hashstr {

	template <typename T = char>
	__attribute__((noinline))
	inline constexpr uint32_t JENKINS(const T* s)
	{
		if (!s) return 0;
		uint32_t hash = 0;
		while (*s) {
			T c = *s++;
			if constexpr (sizeof(T) == 1) {
				if (c >= 'A' && c <= 'Z') c += 0x20;
			}
			else {
				if (c >= static_cast<T>(L'A') && c <= static_cast<T>(L'Z'))
					c = static_cast<T>(c + 0x20);
			}
			hash += static_cast<uint32_t>(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	template <typename T = char>
	__attribute__((noinline))
	inline constexpr uint32_t FNV1A(const T* s)
	{
		if (!s)
			return 0;
		constexpr uint32_t fnv_offset = 2166136261U;
		constexpr uint32_t fnv_prime  = 16777619U;
		uint32_t hash = fnv_offset;
		while (*s) {
			T c = *s++;
			if constexpr (sizeof(T) == 1) {
				if (c >= 'A' && c <= 'Z')
					c += 0x20;
			} else {
				if (c >= L'A' && c <= L'Z')
					c += 0x20;
			}
			hash ^= static_cast<uint32_t>(c);
			hash *= fnv_prime;
		}
		return hash;
	}
}

#define HASH_STR( x ) ( hashstr::JENKINS( x ) )

#endif // __LIB_HASHSTR_H
