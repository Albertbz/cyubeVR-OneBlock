#pragma once
#include "GameAPI.h"
#include "ModBlocks.h"

// A simple struct to keep track of block choices.
// Has a block and its chance of being placed.
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

// A simple struct to keep track of loot choices.
// Has a type, a minAmount and a maxAmount.
struct LootChoice {
	// The type of the loot.
	BlockInfo type;
	// The minimum amount of the loot.
	int minAmount;
	// The maximum amount of the loot.
	int maxAmount;

	/*
	* Constructor for the LootChoice struct.
	* 
	* @param type The type of the loot.
	* @param minAmount The minimum amount of the loot.
	* @param maxAmount The maximum amount of the loot.
	* @param poolNym The pool that the loot is in.
	*/
	LootChoice(BlockInfo type, int minAmount, int maxAmount);
};

// A simple struct to keep track of some loot.
// Has a type and an amount.
struct Loot {
	// The type of the loot.
	BlockInfo type;
	// The amount of the loot.
	int amount;

	/*
	* Constructor for the Loot struct.
	* 
	* @param type The type of the loot.
	* @param amount The amount of the loot.
	*/
	Loot(BlockInfo type, int amount);
};

struct Pool {
	// The loot choices in the pool.
	std::vector<LootChoice> lootChoices;
	// The chance of the pool being picked.
	int chance;

	/*
	* Constructor for the Pool struct.
	* 
	* @param lootChoices The loot choices in the pool.
	* @param chance The chance of the pool being picked.
	*/
	Pool(std::vector<LootChoice> lootChoices, int chance);
};

/*
* Figures out what BlockInfo a given wstring corresponds to (for
* loot).
* 
* @param typeString The type given in wstring format.
* 
* @return The BlockInfo version of the given type.
*/
BlockInfo getLootBlockInfoFromWString(std::wstring typeString);

/*
* Figures out what BlockInfo a given wstring corresponds to (for
* the OneBlock).
* 
* @param blockString The block given in wstring format.
* 
* @return The BlockInfo version of the given block.
*/
BlockInfo getBlockBlockInfoFromWString(std::wstring blockString);

/*
* Converts a BlockInfo into a wstring (for loot).
* 
* @param type The type of the loot.
* 
* @return The wstring version of the given type.
*/
std::wstring getLootWStringFromBlockInfo(BlockInfo type);

/*
* Gets the plural version of the given wstring (for loot).
* 
* @param type The type of the loot.
* 
* @return The plural version of the type.
*/
std::wstring getPluralWString(std::wstring type);

/*
* Gets the native drop from a given BlockInfo.
* 
* @param type The type of the block.
* 
* @return The native drop of the given type.
*/
BlockInfo getNativeDropFromBlockInfo(BlockInfo type);

/*
* Gets the custom drop from a given BlockInfo.
* 
* @param type The type of the block.
* 
* @return The custom drop of the given type.
*/
BlockInfo getCustomDropFromBlockInfo(BlockInfo type);

struct PossibleLocation {
	// The location.
	CoordinateInCentimeters location;
	// Whether it is possible.
	bool possible;


	PossibleLocation(CoordinateInCentimeters location, bool possible);
};