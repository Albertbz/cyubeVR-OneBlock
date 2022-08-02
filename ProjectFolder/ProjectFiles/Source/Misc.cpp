#include "Misc.h"
#include <unordered_map>

BlockChoice::BlockChoice(BlockInfo block, int chance)
{
	this->block = block;
	this->chance = chance;
}

BlockInfo getBlockInfoFromWString(std::wstring blockString)
{
	static std::unordered_map<std::wstring, BlockInfo> const table =
	{
		{L"Stone", BlockInfo(EBlockType::Stone)},
		{L"Grass", BlockInfo(EBlockType::Grass)},
		{L"Dirt", BlockInfo(EBlockType::Dirt)},
		{L"WoodLogDark", BlockInfo(EBlockType::TreeWood)},
		{L"WoodLogLight", BlockInfo(EBlockType::TreeWoodBright)},
		{L"Sand", BlockInfo(EBlockType::Sand)},
		{L"Coal", BlockInfo(EBlockType::Ore_Coal)},
		{L"Iron", BlockInfo(EBlockType::Ore_Iron)},
		{L"Copper", BlockInfo(EBlockType::Ore_Copper)},
		{L"Gold", BlockInfo(EBlockType::Ore_Gold)},
		{L"WoodPlankDark", BlockInfo(EBlockType::WoodPlank)},
		{L"WoodPlankLight", BlockInfo(EBlockType::WoodPlankBright)},
		{L"Crystal", BlockInfo(EBlockType::CrystalBlock)},
		{L"Loot", BlockInfo(677526084)}
	};
	auto it = table.find(blockString);
	if (it != table.end()) {
		return it->second;
	}
	else { 
		return BlockInfo(EBlockType::Air); 
	}
}
