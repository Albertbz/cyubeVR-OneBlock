#include "Misc.h"
#include <unordered_map>

BlockChoice::BlockChoice(BlockInfo block, int chance)
{
	this->block = block;
	this->chance = chance;
}

LootChoice::LootChoice(BlockInfo type, int minAmount, int maxAmount)
{
	this->type = type;
	this->minAmount = minAmount;
	this->maxAmount = maxAmount;
}

Loot::Loot(BlockInfo type, int amount)
{
	this->type = type;
	this->amount = amount;
}

BlockInfo getLootBlockInfoFromWString(std::wstring typeString)
{
	static std::unordered_map<std::wstring, BlockInfo> const table =
	{
		// All native types.
		{L"Stone", BlockInfo(EBlockType::Stone)},
		{L"Grass", BlockInfo(EBlockType::Grass)},
		{L"Dirt", BlockInfo(EBlockType::Dirt)},
		{L"WoodLogDark", BlockInfo(EBlockType::TreeWood)},
		{L"WoodLogBirch", BlockInfo(EBlockType::TreeWoodBright)},
		{L"Sand", BlockInfo(EBlockType::Sand)},
		{L"CoalOre", BlockInfo(EBlockType::Ore_Coal)},
		{L"IronOre", BlockInfo(EBlockType::Ore_Iron)},
		{L"CopperOre", BlockInfo(EBlockType::Ore_Copper)},
		{L"GoldOre", BlockInfo(EBlockType::Ore_Gold)},
		{L"CrystalOre", BlockInfo(EBlockType::CrystalBlock)},
		{L"WoodPlankDark", BlockInfo(EBlockType::WoodPlank)},
		{L"WoodPlankBirch", BlockInfo(EBlockType::WoodPlankBright)},
		{L"CoalNugget", BlockInfo(EBlockType::Nugget_Coal)},
		{L"IronIngot", BlockInfo(EBlockType::IngotIron)},
		{L"CopperNugget", BlockInfo(EBlockType::Nugget_Copper)},
		{L"GoldNugget", BlockInfo(EBlockType::Nugget_Gold)},
		{L"Crystal", BlockInfo(EBlockType::Crystal)},
		{L"Torch", BlockInfo(EBlockType::Torch)},
		{L"TorchBlue", BlockInfo(EBlockType::TorchBlue)},
		{L"TorchGreen", BlockInfo(EBlockType::TorchGreen)},
		{L"TorchRed", BlockInfo(EBlockType::TorchRed)},
		{L"TorchRainbow", BlockInfo(EBlockType::TorchRainbow)},
		{L"DyeBlue", BlockInfo(EBlockType::DyeBlue)},
		{L"DyeGreen", BlockInfo(EBlockType::DyeGreen)},
		{L"DyeRed", BlockInfo(EBlockType::DyeRed)},
		{L"DyeRainbow", BlockInfo(EBlockType::DyeRainbow)},
		{L"DyeWhite", BlockInfo(EBlockType::DyeWhite)},
		{L"Wallstone", BlockInfo(EBlockType::Wallstone)},
		{L"Flagstone", BlockInfo(EBlockType::Flagstone)},
		{L"Scaffolding", BlockInfo(EBlockType::WoodScaffolding)},
		{L"GlassBlock", BlockInfo(EBlockType::GlassBlock)},
		{L"GlassIngot", BlockInfo(EBlockType::GlassIngot)},
		{L"Chest", BlockInfo(EBlockType::Chest1)},
		{L"Chair1Dark", BlockInfo(EBlockType::Chair1)},
		{L"Chair1Birch", BlockInfo(EBlockType::Chair1Birch)},
		{L"Wallmount", BlockInfo(EBlockType::WallmountCopper)},
		{L"FrameWood", BlockInfo(EBlockType::FrameWood)},
		{L"FrameCopper", BlockInfo(EBlockType::FrameCopper)},
		{L"FrameGold", BlockInfo(EBlockType::FrameGold)},
		{L"Furnace", BlockInfo(EBlockType::Furnace)},
		{L"FurnaceMold", BlockInfo(EBlockType::FurnaceMoldIron)},
		{L"WoodStool", BlockInfo(EBlockType::WoodStool)},
		{L"WoodTable1", BlockInfo(EBlockType::WoodTable1)},
		{L"WoodTable2", BlockInfo(EBlockType::WoodTable2)},
		{L"WoodCarafe", BlockInfo(EBlockType::WoodCarafe)},
		{L"WoodBench1", BlockInfo(EBlockType::WoodBench1)},
		{L"WoodBench2", BlockInfo(EBlockType::WoodBench2)},
		{L"WoodBench3", BlockInfo(EBlockType::WoodBench3)},
		{L"WoodBarrel", BlockInfo(EBlockType::WoodBarrel)},
		{L"WoodPost", BlockInfo(EBlockType::WoodPost)},
		{L"MetalPot", BlockInfo(EBlockType::MetalPod)},
		{L"Sandbag", BlockInfo(EBlockType::SandbagPile)},
		{L"Statue", BlockInfo(EBlockType::SmoothbrainStatue)}
	};
	auto it = table.find(typeString);
	if (it != table.end()) {
		return it->second;
	}
	else { 
		return BlockInfo(); 
	}
}

BlockInfo getBlockBlockInfoFromWString(std::wstring blockString)
{
	static std::unordered_map<std::wstring, BlockInfo> const table =
	{
		// All mod blocks.
		{L"Loot", BlockInfo(lootBlockID)},
		{L"Stone", BlockInfo(stoneBlockID)},
		{L"Grass", BlockInfo(grassBlockID)},
		{L"Dirt", BlockInfo(dirtBlockID)},
		{L"WoodLogDark", BlockInfo(woodLogDarkBlockID)},
		{L"WoodLogBirch", BlockInfo(woodLogBirchBlockID)},
		{L"Sand", BlockInfo(sandBlockID)},
		{L"WoodLogDark", BlockInfo(woodLogDarkBlockID)},
		{L"CoalOre", BlockInfo(EBlockType::Ore_Coal)},
		{L"IronOre", BlockInfo(EBlockType::Ore_Iron)},
		{L"CopperOre", BlockInfo(EBlockType::Ore_Copper)},
		{L"GoldOre", BlockInfo(EBlockType::Ore_Gold)},
		{L"CrystalOre", BlockInfo(EBlockType::CrystalBlock)}
	};
	auto it = table.find(blockString);
	if (it != table.end()) {
		return it->second;
	}
	else {
		return BlockInfo();
	}
}
