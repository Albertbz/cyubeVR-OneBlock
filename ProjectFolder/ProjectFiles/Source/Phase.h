#pragma once
#include "GameAPI.h"
#include "Misc.h"

// A class to keep track of a phase.
// Has a name, a start, an end, some blockChoices
// and some pools.
class Phase
{
public:
	// The name of the phase.
	std::wstring name;
	// The point at which the phase starts.
	int start;
	// The point at which the phase ends.
	int end;
	// All of the choices when it comes to blocks. Has the blocks and the chance of them being placed.
	std::vector<BlockChoice> blockChoices;
	// All of the choices when it comes to loot. Has the loot types and their min and max amounts.
	std::vector<Pool> pools;

	// Simple constructor that makes an "empty" phase.
	Phase();

	/*
	* Constructor for the Phase class.
	* 
	* @param path The path of the file containing the phase.
	*/
	Phase(std::wstring path);

	/*
	* Chooses a random block from the phase, with chances
	* taken into account.
	* 
	* @return A randomly chosen block from the phase.
	*/
	BlockInfo getRandomBlock();

	/*
	* Checks whether the given amount destroyed is in the phase.
	* 
	* @param amountDestroyed The amount of times the OneBlock has been destroyed.
	* 
	* @return Whether the given amountDestroyed is in the phase.
	*/
	bool isInPhase(int amountDestroyed);

	/*
	* Chooses some random amount of loot from the possible
	* loot in current phase.
	* 
	* @return A vector with the loot.
	*/
	std::vector<Loot> getRandomLoot();
};