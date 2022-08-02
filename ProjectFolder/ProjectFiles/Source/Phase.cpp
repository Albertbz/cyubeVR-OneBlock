#include "Phase.h"
#include <iostream>
#include <fstream>

Phase::Phase()
{
	this->name = L"Empty Phase";
	this->start = 0;
	this->end = 0;
	this->blockChoices = {};
}

Phase::Phase(std::wstring path)
{
	// Uses the path to load the phase, starting with the name, start and
	// end of the phase, and then lastly all of the blocks and their chances.

	std::wifstream file(path);
	std::wstring line;

	// Get the name.
	std::getline(file, line);
	this->name = line;

	// Get the start.
	std::getline(file, line);
	this->start = std::stoi(line);

	// Get the end.
	std::getline(file, line);
	this->end = std::stoi(line);

	// Get all of the blocks and their chances.
	std::vector<BlockChoice> blockChoices;
	while (std::getline(file, line)) { // For all remaining lines.
		size_t pos = line.find_first_of(L" ");

		// Get the block.
		std::wstring blockString = std::wstring(line, 0, pos);
		BlockInfo block = getBlockInfoFromWString(blockString);
		
		// Get the chance.
		std::wstring chanceString = std::wstring(line, pos + 1);
		int chance = std::stoi(chanceString);

		blockChoices.push_back(BlockChoice(block, chance));
	}
	this->blockChoices = blockChoices;
}

BlockInfo Phase::getRandomBlock()
{
	// Chooses a random number between 1 and 100, and then goes through
	// the possible blocks and adds their chance to see if they are to
	// be chosen.

	int randomNum = GetRandomInt_2<1, 100>();
	int currentNum = 0;
	for (BlockChoice bc : blockChoices) {
		currentNum += bc.chance;
		if (randomNum <= currentNum) {
			return bc.block;
		}
	}
	return BlockInfo(EBlockType::Air);
}

bool Phase::isInPhase(int amountDestroyed)
{
	return amountDestroyed <= end && amountDestroyed >= start;
}
