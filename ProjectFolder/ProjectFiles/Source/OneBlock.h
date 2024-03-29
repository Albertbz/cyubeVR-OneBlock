#pragma once
#include "GameAPI.h"
#include "Phase.h"

// The main class for this project.
class OneBlock
{
public:
	// Whether the currently active world is a OneBlock world.
	bool isOneBlock;
	// The center/where the OneBlock is.
	CoordinateInBlocks center;
	// The amount of times the OneBlock has been destroyed.
	int amountDestroyed;
	// All OneBlock phases.
	std::vector<Phase> phases;
	// The phase that the world is currently in.
	Phase currentPhase;
	// Whether the player is currently being asked if they want to create the OneBlock world.
	static bool askingIfCreate;
	// A vector with all currently active hintTextHandles.
	static std::vector<void*> hintTextHandles;
	// Used for showing amount destroyed and all other hint texts at the same location.
	void* currentHintTextHandle;
	// Whether to ignore when a block is placed.
	static bool ignoreBlockPlacement;
	// Whether to ignore when a block is destroyed.
	static bool ignoreBlockDestroyment;
	// The drops from the most recent time the OneBlock was destroyed.
	std::vector<BlockInfoWithLocation> drops;
	// Whether currently replacing drops.
	bool isReplacingDrops;
	// The previous block that was destroyed.
	BlockInfo previousBlock;
	// Whether the player is currently moving a block from (0, 0, 200).
	bool isMovingBlock;

	// Constructor for the OneBlock class.
	OneBlock();

	// Loads the progress of the currently active OneBlock world.
	void loadProgress();

	// Loads the phases from the install folder into the phases field.
	void loadPhases();

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

	/*
	* Updates the phase to the one it's supposed to be according to the amount
	* of blocks destroyed - and prints the phase if told to (only used when
	* new phase).
	* 
	* @param printPhase Whether to print the "Welcome to phase X" message.
	*/
	void updateCurrentPhase(bool printPhase);

	/*
	* Checks whether the OneBlock world already exists.
	*
	* @return Whether the OneBlock world already exists.
	*/
	static bool exists();

	/*
	* Creates the OneBlock world by copying the world files into the WorldData
	* folder.
	* 
	* @return Whether it was succesful.
	*/
	static bool create();

	// Destroys all hint texts in the hintTextHandles.
	static void destroyHintTexts();

	/*
	* Removes old hint text (currentHintTextHandle) and spawns the new one.
	*
	* @param location The location to spawn the hint text at.
	* @param text The text to have on the hint text.
	* @param duration The duration of the hint text in seconds, default -1 (infinite).
	* @param sizeMul The size multiplier for the size of the hint text, default 1.
	* @param sizeMulVer The size multiplier for the vertical size of the hint text, default 1.
	* @param fontMul The size multiplier for the font of the hint text, default 1.
	*/
	void printHintText(CoordinateInCentimeters location, std::wstring text, float duration = -1, float sizeMul = 1, float sizeMulVer = 1, float fontMul = 1);

	/*
	* Checks whether the location given is out of bounds, with out of bounds being
	* either below layer 10 or more than 600 blocks out in any direction from 0,0.
	* 
	* @return Whether the player is out of bounds.
	*/
	static bool isOutOfBounds(CoordinateInBlocks location);

	/*
	* Calculates the amount of digits in a given number.
	* 
	* @param number The number to get the amount of digits of.
	* 
	* @return The amount of digits in number.
	*/
	static int numDigits(int number);

	// Gives the player loot from the current phase.
	void giveLoot();

	/*
	* Calculates where on a circle of the given radius from 0,0 that a 
	* hint text should be spawned.
	* 
	* @param height The height offset for the location.
	* @param radius	The radius of the circle.
	* 
	* @return The location that a hint text should be spawned at.
	*/
	CoordinateInCentimeters getHintTextLocation(int height, int radius);

	// Removes the drops that were already spawned. Updates the
	// drops field.
	void removeDrops();

	// Spawns the drops that correspond to the block that was just destroyed
	// at the center block.
	void spawnCustomDrops();
};