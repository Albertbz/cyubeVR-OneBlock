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


Pool::Pool(std::vector<LootChoice> lootChoices, int poolNum)
{
	this->lootChoices = lootChoices;
	this->poolNum = poolNum;
}

BlockInfo getLootBlockInfoFromWString(std::wstring typeString)
{
	static std::unordered_map<std::wstring, BlockInfo> const table =
	{
		// All native types.
		{L"Stone", BlockInfo(EBlockType::StoneMined)}, // StoneMined because this is for AddToInventory purposes.
		{L"Grass", BlockInfo(EBlockType::Grass)}, // Shouldn't be used.
		{L"Dirt", BlockInfo(EBlockType::Dirt)},
		{L"WoodLogDark", BlockInfo(EBlockType::TreeWood)},
		{L"WoodLogBirch", BlockInfo(EBlockType::TreeWoodBright)},
		{L"Sand", BlockInfo(EBlockType::Sand)},
		{L"CoalOre", BlockInfo(EBlockType::Ore_Coal)}, // Shouldn't be used.
		{L"IronOre", BlockInfo(EBlockType::Ore_Iron)},
		{L"CopperOre", BlockInfo(EBlockType::Ore_Copper)}, // Shouldn't be used.
		{L"GoldOre", BlockInfo(EBlockType::Ore_Gold)}, // Shouldn't be used.
		{L"CrystalOre", BlockInfo(EBlockType::CrystalBlock)}, // Shouldn't be used.
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
		{L"Statue", BlockInfo(EBlockType::SmoothbrainStatue)},
		{L"DryGrass", BlockInfo(EBlockType::DryGrass)}
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
		// All placeable blocks.
		{L"Loot", BlockInfo(lootBlockID)},
		{L"Stone", BlockInfo(stoneBlockID)},
		{L"Grass", BlockInfo(grassBlockID)},
		{L"Dirt", BlockInfo(dirtBlockID)},
		{L"WoodLogDark", BlockInfo(woodLogDarkBlockID)},
		{L"WoodLogBirch", BlockInfo(woodLogBirchBlockID)},
		{L"Sand", BlockInfo(sandBlockID)},
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

std::wstring getLootWStringFromBlockInfo(BlockInfo type)
{
	static std::unordered_map<EBlockType, std::wstring> const table =
	{
		// All native types.
		{EBlockType::StoneMined, L"Stone"},
		{EBlockType::Grass, L"Grass"}, // Shouldn't be used.
		{EBlockType::Dirt, L"Dirt"},
		{EBlockType::TreeWood, L"Dark Wood Log"},
		{EBlockType::TreeWoodBright, L"Birch Wood Log"},
		{EBlockType::Sand, L"Sand"},
		{EBlockType::Ore_Coal, L"Coal Ore"}, // Shouldn't be used.
		{EBlockType::Ore_Iron, L"Iron Ore"},
		{EBlockType::Ore_Copper, L"Copper Ore"}, // Shouldn't be used.
		{EBlockType::Ore_Gold, L"Gold Ore"}, // Shouldn't be used.
		{EBlockType::CrystalBlock, L"Crystal Ore"}, // Shouldn't be used.
		{EBlockType::WoodPlank, L"Dark Wood Plank"},
		{EBlockType::WoodPlankBright, L"Birch Wood Plank"},
		{EBlockType::Nugget_Coal, L"Coal Nugget"},
		{EBlockType::IngotIron, L"Iron Ingot"},
		{EBlockType::Nugget_Copper, L"Copper Nugget"},
		{EBlockType::Nugget_Gold, L"Gold Nugget"},
		{EBlockType::Crystal, L"Crystal"},
		{EBlockType::Torch, L"Torch"},
		{EBlockType::TorchBlue, L"Blue Torch"},
		{EBlockType::TorchGreen, L"Green Torch"},
		{EBlockType::TorchRed, L"Red Torch"},
		{EBlockType::TorchRainbow, L"Rainbow Torch"},
		{EBlockType::DyeBlue, L"Blue Dye"},
		{EBlockType::DyeGreen, L"Green Dye"},
		{EBlockType::DyeRed, L"Red Dye"},
		{EBlockType::DyeRainbow, L"Rainbow Dye"},
		{EBlockType::DyeWhite, L"White Dye"},
		{EBlockType::Wallstone, L"Wallstone"},
		{EBlockType::Flagstone, L"Flagstone"},
		{EBlockType::WoodScaffolding, L"Scaffolding"},
		{EBlockType::GlassBlock, L"Glass Block"},
		{EBlockType::GlassIngot, L"Glass Ingot"},
		{EBlockType::Chest1, L"Chest"},
		{EBlockType::Chair1, L"Dark Chair"},
		{EBlockType::Chair1Birch, L"Birch Chair"},
		{EBlockType::WallmountCopper, L"Wallmount"},
		{EBlockType::FrameWood, L"Wooden Frame"},
		{EBlockType::FrameCopper, L"Copper Frame"},
		{EBlockType::FrameGold, L"Gold Frame"},
		{EBlockType::Furnace, L"Furnace"},
		{EBlockType::FurnaceMoldIron, L"Furnace Mold"},
		{EBlockType::WoodStool, L"Wood Stool"},
		{EBlockType::WoodTable1, L"Wood Table"},
		{EBlockType::WoodTable2, L"Wood Table"},
		{EBlockType::WoodCarafe, L"Wood Carafe"},
		{EBlockType::WoodBench1, L"Wood Bench"},
		{EBlockType::WoodBench2, L"Wood Bench"},
		{EBlockType::WoodBench3, L"Wood Bench"},
		{EBlockType::WoodBarrel, L"Wood Barrel"},
		{EBlockType::WoodPost, L"Wood Post"},
		{EBlockType::MetalPod, L"Metal Pot"},
		{EBlockType::SandbagPile, L"Sandbag"},
		{EBlockType::SmoothbrainStatue, L"Statue"},
		{EBlockType::DryGrass, L"Dry Grass"}
	};
	auto it = table.find(type.Type);
	if (it != table.end()) {
		return it->second;
	}
	else {
		return std::wstring();
	}
}

std::wstring getPluralWString(std::wstring type)
{
	static std::unordered_map<std::wstring, std::wstring> const table =
	{
		// All types that have plural versions.
		{L"Dark Wood Log", L"Dark Wood Logs"},
		{L"Birch Wood Log", L"Birch Wood Logs"},
		{L"Dark Wood Plank", L"Dark Wood Planks"},
		{L"Birch Wood Plank", L"Birch Wood Planks"},
		{L"Coal Nugget", L"Coal Nuggets"},
		{L"Iron Ingot", L"Iron Ingots"},
		{L"Copper Nugget", L"Copper Nuggets"},
		{L"Gold Nugget", L"Gold Nuggets"},
		{L"Crystal", L"Crystals"},
		{L"Torch", L"Torches"},
		{L"Blue Torch", L"Blue Torches"},
		{L"Green Torch", L"Green Torches"},
		{L"Red Torch", L"Red Torches"},
		{L"Rainbow Torch", L"Rainbow Torches"},
		{L"Glass Block", L"Glass Blocks"},
		{L"Glass Ingot", L"Glass Ingots"},
		{L"Chest", L"Chests"},
		{L"Dark Chair", L"Dark Chairs"},
		{L"Birch Chair", L"Birch Chairs"},
		{L"Wallmount", L"Wallmounts"},
		{L"Wooden Frame", L"Wooden Frames"},
		{L"Copper Frame", L"Copper Frames"},
		{L"Gold Frame", L"Gold Frames"},
		{L"Furnace", L"Furnaces"},
		{L"Furnace Mold", L"Furnace Molds"},
		{L"Wood Stool", L"Wood Stools"},
		{L"Wood Table", L"Wood Tables"},
		{L"Wood Carafe", L"Wood Carafes"},
		{L"Wood Bench", L"Wood Benches"},
		{L"Wood Barrel", L"Wood Barrels"},
		{L"Wood Post", L"Wood Posts"},
		{L"Metal Pot", L"Metal Pots"},
		{L"Sandbag", L"Sandbags"},
		{L"Statue", L"Statues"}
	};
	auto it = table.find(type);
	if (it != table.end()) {
		return it->second;
	}
	else {
		return type;
	}
}
