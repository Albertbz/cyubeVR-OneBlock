#include "GameAPI.h"
#include "OneBlock.h"

/************************************************************
	Custom variables for OneBlock
*************************************************************/

// The current OneBlock instance for the world.
OneBlock world;

// Number of ticks since world was loaded. Used for askIfOneBlock method.
int ticksSinceLoad;
// Where the player spawned when creating the world if it's a new world.
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
	if (OneBlock::askingIfCreate) {
		if (ticksSinceLoad < 600) {
			OneBlock::askIfCreate(false, initialSpawn);
			ticksSinceLoad++;
		}
		else {
			OneBlock::askingIfCreate = false;
		}
	}
}



// Run once when the world is loaded
void Event_OnLoad(bool CreatedNewWorld)
{
	if (OneBlock::exists()) {
		OneBlock::askingIfCreate = false;
		world.load();
	}
	else {
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

}

// Run every time any block is destroyed by the player
void Event_AnyBlockDestroyed(CoordinateInBlocks At, BlockInfo Type, bool Moved)
{
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