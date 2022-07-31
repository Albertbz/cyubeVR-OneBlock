#include "GameAPI.h"
#include "OneBlock.h"

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

UniqueID ThisModUniqueIDs[] = { 0 }; // All the UniqueIDs this mod manages. Functions like Event_BlockPlaced are only called for blocks of IDs mentioned here. 

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
			SetPlayerLocation(world.center + CoordinateInBlocks(0, 0, 1));
			world.printHintText(GetPlayerLocationHead() + GetPlayerViewDirection(), L"You were out of bounds and have\nbeen teleported back to the center.", 10);
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
		if (OneBlock::isOutOfBounds(At)) {
			SetBlock(At, EBlockType::Air);
			AddToInventory(Type, 1);
			world.printHintText(GetPlayerLocationHead() + GetPlayerViewDirection(), L"That block was out of bounds.", 5);
		}
	}
}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{
	// If the active world is a OneBlock world, and the destroyed block was the center block, then
	// increment the amount of times it has been destroyed, print this amount, and set a new block.
	if (world.isOneBlock) {
		if (At == world.center) {
			world.incrementAmount();
			world.printAmountDestroyed();
			world.setOneBlock();
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