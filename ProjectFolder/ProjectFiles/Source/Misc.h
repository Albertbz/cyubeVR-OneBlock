#pragma once
#include "GameAPI.h"

struct BlockChoice {
	// The block.
	BlockInfo block;
	// The chance of it being placed.
	int chance;

	/*
	* Constructor for the BlockChance struct.
	*
	* @param block The block.
	* @param chance The chance of it being placed.
	*/
	BlockChoice(BlockInfo block, int chance);
};

/*
* Figures out what BlockInfo a given wstring corresponds to (for
* certain blocks)
* 
* @param blockString The block given in wstring format.
* 
* @return The BlockInfo version of the given block.
*/
BlockInfo getBlockInfoFromWString(std::wstring blockString);


/******************************************************
 Moved functions from GameAPI.cpp because they can only 
 be used in Mod.cpp otherwise.
*******************************************************/

template<class T>
constexpr auto absolute_2(T const& x) {
	return x < 0 ? -x : x;
}

constexpr static __forceinline uint64_t rotl_2(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

static std::random_device rd_2;
static std::mt19937 rng_2(rd_2());
static uint64_t xors_s_2[2] = { std::uniform_int_distribution<uint64_t>(0, UINT64_MAX)(rng_2), std::uniform_int_distribution<uint64_t>(0, UINT64_MAX)(rng_2) };

__forceinline uint64_t xoroshiro128p_2(void) {
	const uint64_t s0 = xors_s_2[0];
	uint64_t s1 = xors_s_2[1];
	const uint64_t result = s0 + s1;

	s1 ^= s0;
	xors_s_2[0] = rotl_2(s0, 24) ^ s1 ^ (s1 << 16); // a, b
	xors_s_2[1] = rotl_2(s1, 37); // c

	return result;
}

template<uint64_t TrueOneInN>
bool GetRandomBool_2()
{
	static constexpr uint64_t AboveThisTrue = UINT64_MAX - (UINT64_MAX / TrueOneInN);

	return xoroshiro128p_2() > AboveThisTrue;
}

template<int32_t Min, int32_t Max>
int32_t GetRandomInt_2()
{
	static_assert(Max > Min, "Called GetRandomInt with Min larger than Max");
	static_assert(Max != INT32_MAX, "GetRandomInt Max can't be INT32_MAX. Please reduce Max by at least one");

	static constexpr uint32_t TotalSpan = int64_t(Max) - int64_t(Min);
	static constexpr uint32_t DivideBy = (UINT32_MAX / (TotalSpan + 1)) + 1;

	if constexpr (TotalSpan == UINT32_MAX) return int32_t(xoroshiro128p_2());
	if constexpr (Min == Max) return Min;

	return int32_t(uint32_t(xoroshiro128p_2()) / DivideBy) + Min;
}