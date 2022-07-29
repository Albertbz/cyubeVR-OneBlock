#pragma once
#include "GameAPI.h"
class OneBlock
{
public:
	// Whether the currently active world is a OneBlock world.
	bool isOneBlock;
	// The center/where the OneBlock is.
	CoordinateInBlocks center;
	// The amount of times the OneBlock has been destroyed.
	int amountDestroyed;
	// The phase that the world is currently in.
	int phase;
	// Whether the player is currently being asked if they want to create the OneBlock world.
	static bool askingIfCreate;
	// A vector with all currently active hintTextHandles.
	static std::vector<void*> hintTextHandles;
	// Used for showing amount destroyed and all other hint texts at the same location.
	void* currentHintTextHandle;

	// Constructor for the OneBlock class.
	OneBlock();

	// Loads the progress of the currently active OneBlock world.
	void loadProgress();

	// Checks whether the currently active world is a OneBlock world and loads accordingly.
	void load();

	/*
	* Asks the player whether to create the OneBlock world.
	*
	* @param printText Whether to print the text that asks.
	* @param initialSpawn The coordinates of where the player spawned.
	*/
	static void askIfCreate(bool printText, CoordinateInCentimeters initialSpawn);

	/*
	* Checks if a hand is in either Yes or No (for askIfCreate).
	*
	* @param leftHand Whether it is the left hand that is being checked.
	* @param initialSpawn The coordinates of where the player spawned.
	*/
	static void checkHand(bool leftHand, CoordinateInCentimeters initialSpawn);

	/*
	* Checks if the given location is between the two given corners.
	*
	* @param corner1 The first corner. Should be the smallest in all coordinates.
	* @param corner2 The second corner. Should be the largest in all coordinates.
	* @param loc The location to check.
	*
	* @return Whether the given location is between the two given corners.
	*/
	static bool isBetween(CoordinateInCentimeters corner1, CoordinateInCentimeters corner2, CoordinateInCentimeters loc);

	// Creates a Void world by removing a ton of blocks, teleporting the player to 0,0
	// and placing a singular block there. After this, the world should be ready.
	// Old method that was only used to make the void world - isn't used anymore.
	void createVoidWorld();

	// Increments the amountDestroyed field.
	void incrementAmount();

	// Spawns a HintText above the OneBlock telling you the amount of blocks destroyed.
	void printAmountDestroyed();

	// Sets the OneBlock according to the current phase.
	void setOneBlock();

	// Updates the phase to the one it's supposed to be according to the amount
	// of blocks destroyed.
	void updatePhase();

	/*
	* Checks whether the OneBlock world already exists.
	*
	* @return Whether the OneBlock world already exists.
	*/
	static bool exists();

	// Creates the OneBlock world by moving the world files into the WorldData
	// folder.
	static void create();

	// Destroys all hint texts in the hintTextHandles.
	static void destroyHintTexts();

	/*
	* Removes old hint text (currentHintTextHandle) and spawns the new one.
	*
	* @param location The location to spawn the hint text at.
	* @param text The text to have on the hint text.
	* @param duration The duration of the hint text in seconds, default -1 (infinite).
	* @param sizeMulHor The size multiplier for the horizontal size of the hint text, default 1.
	* @param sizeMulVer The size multiplier for the vertical size of the hint text, default 1.
	*/
	void printHintText(CoordinateInCentimeters location, std::wstring text, float duration = -1, float sizeMul = 1.0F, float sizeMulVer = 1.0F);
};