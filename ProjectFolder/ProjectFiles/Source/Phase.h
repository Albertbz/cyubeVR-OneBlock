#pragma once
#include "GameAPI.h"
#include "Misc.h"

class Phase
{
public:
	// The name of the phase.
	std::wstring name;
	// The point at which the phase starts.
	int start;
	// The point at which the phase ends.
	int end;
	// All of the choices when it comes to block. Has the block and the chance of it being placed.
	std::vector<BlockChoice> blockChoices;

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
};