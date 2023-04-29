#include "GameAPI.h"
#include "OneBlock.h"
#include <chrono>
#include <thread>

/************************************************************
	Custom variables for OneBlock
*************************************************************/

// The current OneBlock instance for the world.
OneBlock world;

// Number of ticks since world was loaded. Used for askIfCreate method.
int ticksSinceLoad;
// Where the player spawned when opening the world.
CoordinateInCentimeters initialSpawn;

/************************************************************
	Config Variables (Set these to whatever you need. They are automatically read by the game.)
*************************************************************/

UniqueID ThisModUniqueIDs[] = { lootBlockID }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

float TickRate = 10;				 // Set how many times per second Event_Tick() is called. 0 means the Event_Tick() function is never called.

/*************************************************************
//	Functions (Run automatically by the game, you can put any code you want into them)
*************************************************************/

// Run every time a block is placed
void Event_BlockPlaced(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{

}


// Run every time a block is destroyed
void Event_BlockDestroyed(CoordinateInBlocks At, UniqueID CustomBlockID, bool Moved)
{
	// Can only ever be triggered when a Loot block is destroyed, so no need
	// to do any checks.
	world.giveLoot();
}


// Run every time a block is hit by a tool
void Event_BlockHitByTool(CoordinateInBlocks At, UniqueID CustomBlockID, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}


// Run X times per second, as specified in the TickRate variable at the top
void Event_Tick()
{
	// If currently asking the player whether to create the OneBlock world,
	// and if it hasn't been more than 1 minute since the world was opened,
	// then keep checking for an answer with askIfCreate.
	if (OneBlock::askingIfCreate) {
		if (ticksSinceLoad < 600) {
			OneBlock::askIfCreate(false, initialSpawn);
			ticksSinceLoad++;
		}
		else {
			OneBlock::askingIfCreate = false;
		}
	}															// Else if already a OneBlock world, then if the player is out
	else if (world.isOneBlock) {								// of bounds, teleport the player back to 0,0 and tell them they
		if (OneBlock::isOutOfBounds(GetPlayerLocation())) {		// were out of bounds.
			SetPlayerLocation(world.center);
			world.printHintText(world.center + (GetPlayerLocationHead() - GetPlayerLocation()) + CoordinateInCentimeters(0, 0, 20) + GetPlayerViewDirection() * 40, L"You were out of bounds and\nhave been teleported back\nto the center.", 10);
		}
		
		if (world.isReplacingDrops) {
			world.isReplacingDrops = false;
			world.removeDrops();
			world.spawnCustomDrops();
		}
	}
}



// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	// If the OneBlock world already exists, simply load it.
	if (OneBlock::exists()) {
		OneBlock::askingIfCreate = false;
		world.load();
	}
	else { // Else, ask the player if they would like to create the OneBlock world.
		ticksSinceLoad = 0;
		initialSpawn = GetPlayerLocationHead();
		OneBlock::askingIfCreate = true;
		OneBlock::askIfCreate(true, initialSpawn);
	}
}

// Run once when the world is exited
void Event_OnExit()
{
	
}

/*******************************************************

	Advanced functions

*******************************************************/


// Run every time any block is placed by the player
void Event_AnyBlockPlaced(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{
	// If the active world is a OneBlock world, and the placed block was out of bounds, then
	// remove the block and give it back to the player as well as tell the player that the
	// block they tried to place was out of bounds.
	if (world.isOneBlock) {
		if (!OneBlock::ignoreBlockPlacement) { // If this check isn't done, doing SetBlock(At, EBlockType::Air) will make it loop forever.
			
			if (OneBlock::isOutOfBounds(At)) {
				OneBlock::ignoreBlockPlacement = true;
				SetBlock(At, EBlockType::Air);
				if (!Moved) {
					AddToInventory(Type, 1);
				}
				world.printHintText(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, L"Block out of bounds.", 5);
			}
			else if (At == world.center + CoordinateInBlocks(0, 0, 1) || At == world.center + CoordinateInBlocks(0, 0, 2) || At == world.center + CoordinateInBlocks(0, 0, 3) || At == world.center + CoordinateInBlocks(0, 0, 4)) {
				OneBlock::ignoreBlockPlacement = true;
				SetBlock(At, EBlockType::Air);
				if (!Moved) {
					AddToInventory(Type, 1);
				}
				world.printHintText(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, L"You can't place a block\non the 4 blocks\nabove the OneBlock.", 5);
			}

			// If currently moving the regenerating block and it is placed somewhere
			// that is not the center, increment counter, etc. Otherwise, do nothing.
			if (Moved && world.isMovingBlock) {
				world.isMovingBlock = false;
				if (At != world.center) {
					world.incrementAmount();
					world.printAmountDestroyed();
				}
			}
		}
		else {
			OneBlock::ignoreBlockPlacement = false;
		}
	}
}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{
	// If the active world is a OneBlock world, and the destroyed block was the center block, then
	// increment the amount of times it has been destroyed, print this amount, and set a new block.
	if (world.isOneBlock) {
		if (!OneBlock::ignoreBlockDestroyment) { // This check is done so that it is possible to change the block via code without incrementing amountDestroyed.
			if (At == world.center) {
				// If not moved, then the block was destroyed normally and everything is done.
				if (!Moved) {
					world.isReplacingDrops = true;
					world.previousBlock = Type;
					world.incrementAmount();
					world.printAmountDestroyed();
				}
				else {
					world.isMovingBlock = true;
				}

				// Always set a new block, even in the case that it is
				// overwritten by a moved block instantly.
				world.setOneBlock();
			}
		}
		else {
			OneBlock::ignoreBlockDestroyment = false;
		}
	}
}

// Run every time any block is hit by a tool
void Event_AnyBlockHitByTool(CoordinateInBlocks At, BlockInfo Type, wString ToolName, CoordinateInCentimeters ExactHitLocation, bool ToolHeldByHandLeft)
{

}


/*******************************************************

	For all the available game functions you can call, look at the GameAPI.h file

*******************************************************/