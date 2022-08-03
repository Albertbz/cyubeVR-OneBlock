#include "Phase.h"
#include <iostream>
#include <fstream>
#include <random>

Phase::Phase()
{
	this->name = L"Empty Phase";
	this->start = 0;
	this->end = 0;
	this->blockChoices = {};
	this->lootChoices = {};
}

Phase::Phase(std::wstring path)
{
	// Uses the path to load the phase, starting with the name, start and
	// end of the phase, then all of the blocks and their chances, and 
	// lastly the loot and their amounts.

	std::wifstream file(path);
	std::wstring line;

	// Get the name.
	std::getline(file, line);
	this->name = line;

	// Get the start and end.
	std::getline(file, line);
	size_t pos = line.find_first_of(L"-");
	this->start = std::stoi(std::wstring(line, 0, pos));
	this->end = std::stoi(std::wstring(line, pos + 1));

	// Get all of the blocks and their chances.
	std::vector<BlockChoice> blockChoices;
	while (std::getline(file, line)) {
		pos = line.find_first_of(L" ");

		if (pos == std::string::npos) {
			break;
		}

		// Get the block.
		std::wstring blockWString = std::wstring(line, 0, pos);
		BlockInfo block = getBlockBlockInfoFromWString(blockWString);
		
		// Get the chance.
		std::wstring chanceWString = std::wstring(line, pos + 1);
		int chance = std::stoi(chanceWString);

		blockChoices.push_back(BlockChoice(block, chance));
	}
	this->blockChoices = blockChoices;

	// Get all of the possible loot and their amounts.
	std::vector<LootChoice> lootChoices;
	while (std::getline(file, line)) {
		pos = line.find_first_of(L" ");

		// Get the loot type.
		std::wstring typeWString = std::wstring(line, 0, pos);
		BlockInfo type = getLootBlockInfoFromWString(typeWString);

		// Get the loot min and max amounts.
		std::wstring minAndMax = std::wstring(line, pos + 1);
		pos = minAndMax.find_first_of(L"-");

		int minAmount = std::stoi(std::wstring(minAndMax, 0, pos));
		int maxAmount = std::stoi(std::wstring(minAndMax, pos + 1));

		lootChoices.push_back(LootChoice(type, minAmount, maxAmount));
	}
	this->lootChoices = lootChoices;
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

std::vector<Loot> Phase::getRandomLoot()
{
	// Goes through the possible loot and chooses a random
	// amount for each loot according to their min and max
	// amounts.

	std::vector<Loot> res;

	std::random_device rd; 
	std::mt19937 gen(rd());

	for (LootChoice lc : lootChoices) {
		std::uniform_int_distribution<> distrib(lc.minAmount, lc.maxAmount);
		int amount = distrib(gen);

		// If amount is 0, don't add it.
		if (amount == 0) {
			continue;
		}

		res.push_back(Loot(lc.type, amount));
	}

	return res;
}
