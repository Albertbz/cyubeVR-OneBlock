#pragma once
#include "GameAPI.h"
class OneBlock
{
public:
	// Whether the currently
	bool isLoaded;
	// Whether the currently active world is a OneBlock world.
	bool isOneBlock;
	// The center/where the OneBlock is.
	CoordinateInBlocks center;
	// The amount of times the OneBlock has been destroyed.
	int amountDestroyed;
	// The phase that the world is currently in.
	int phase;

	// Constructor for the OneBlock class.
	OneBlock();

	// Loads the progress of the currently active OneBlock world.
	void loadProgress();

	// Checks whether the currently active world is a OneBlock world and loads accordingly.
	void load();

	/*
	* Asks the player whether the newly created world is a OneBlock world and
	* if yes, makes it one.
	* 
	* @param printText Whether to print the text that asks.
	* @param initialSpawn The coordinates of where the player spawned.
	*/
	void askIfOneBlock(bool printText, CoordinateInCentimeters initialSpawn);

	/*
	* Checks if a hand is in either Yes or No (for askIfOneBlock).
	* 
	* @param leftHand Whether it is the left hand that is being checked.
	* @param initialSpawn The coordinates of where the player spawned.
	*/
	void checkHand(bool leftHand, CoordinateInCentimeters initialSpawn);

	/*
	* Checks if the given location is between the two given corners.
	* 
	* @param corner1 The first corner. Should be the smallest in all coordinates.
	* @param corner2 The second corner. Should be the largest in all coordinates.
	* @param loc The location to check.
	* 
	* @return Whether the given location is between the two given corners.
	*/
	bool isBetween(CoordinateInCentimeters corner1, CoordinateInCentimeters corner2, CoordinateInCentimeters loc);

	// Creates the OneBlock world by removing a ton of blocks, teleporting the player to 0,0
	// and placing the singular block there. After this, the world should be ready.
	// Old method that was only used to make the void world - isn't used anymore.
	void createOneBlock();

	// Increments the amountDestroyed field.
	void incrementAmount();

	// Spawns a HintText above the OneBlock telling you the amount of blocks destroyed.
	void printAmountDestroyed();

	// Sets the OneBlock according to the current phase.
	void setOneBlock();

	// Updates the phase to the one it's supposed to be according to the amount
	// of blocks destroyed.
	void updatePhase();
};


