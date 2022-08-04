#include "Phase.h"
#include <iostream>
#include <fstream>
#include <random>

Phase::Phase()
{
	this->name = L"Empty Phase";
	this->description = L"Empty Description";
	this->start = 0;
	this->end = 0;
	this->blockChoices = {};
	this->pools = {};
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

	// Get the description.
	std::getline(file, line);
	std::wstring description;
	size_t pos = line.find(L"\\n");
	while (pos != std::wstring::npos) {
		description = description + std::wstring(line, 0, pos) + L"\n";
		line = std::wstring(line, pos + 2);
		pos = line.find(L"\n");
	}
	description = description + line;
	this->description = description;

	// Get the start and end.
	std::getline(file, line);
	pos = line.find_first_of(L"-");
	this->start = std::stoi(std::wstring(line, 0, pos));
	int end = std::stoi(std::wstring(line, pos + 1));
	end == -1 ? this->end = INT_MAX : this->end = end; // If end is -1, set it to the max.

	// Get all of the blocks and their chances.
	std::vector<BlockChoice> blockChoices;
	while (std::getline(file, line)) {
		pos = line.find_first_of(L" ");

		if (pos == std::wstring::npos) {
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
	std::vector<Pool> pools;
	int poolNum = 1;
	while (std::getline(file, line)) {
		pos = line.find_first_of(L" ");

		// If there is no space, i.e., there's an empty line, then it is
		// the end of the current pool.
		if (pos == std::wstring::npos) {
			pools.push_back(Pool(lootChoices, poolNum));
			lootChoices = {};
			poolNum++;
			continue;
		}

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
	pools.push_back(Pool(lootChoices, poolNum));
	this->pools = pools;
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
	// pool and amount for each loot according to their min and max
	// amounts.

	std::vector<Loot> res;

	std::random_device rd; 
	std::mt19937 gen(rd());

	// Figure out which pool to use.
	int numOfPools = (int)pools.size();
	std::uniform_int_distribution<> distrib1(1, numOfPools);
	int randomPoolNum = distrib1(gen);
	Pool randomPool = Pool(std::vector<LootChoice>(), 0);

	// Find the pool.
	for (Pool p : pools) {
		if (p.poolNum == randomPoolNum) {
			randomPool = p;
			break;
		}
	}

	// Choose random amounts of loot from pool.
	for (LootChoice lc : randomPool.lootChoices) {
		std::uniform_int_distribution<> distrib2(lc.minAmount, lc.maxAmount);
		int amount = distrib2(gen);

		// If amount is 0, don't add it.
		if (amount == 0) {
			continue;
		}

		res.push_back(Loot(lc.type, amount));
	}

	return res;
}
