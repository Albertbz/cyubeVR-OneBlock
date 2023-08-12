#include "OneBlock.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <math.h>

bool OneBlock::askingIfCreate = false;
std::vector<void*> OneBlock::hintTextHandles;
bool OneBlock::ignoreBlockPlacement = false;
bool OneBlock::ignoreBlockDestroyment = false;

OneBlock::OneBlock()
{
	// All default values for the fields.
	this->isOneBlock = false;
	this->center = CoordinateInBlocks(0, 0, 200);
	this->amountDestroyed = 0;
	this->phases = {};
	this->currentPhase = Phase();
	this->currentHintTextHandle = nullptr;
	this->drops = std::vector<BlockInfoWithLocation>();
	this->isReplacingDrops = false;
	this->previousBlock = BlockInfo();
	this->isMovingBlock = false;
}

void OneBlock::loadProgress()
{
	// Get amountDestroyed from the world save folder.
	std::wstring loadedString;
	if (LoadModDataString(L"OneBlock\\amountDestroyed", loadedString)) {
		amountDestroyed = std::stoi(loadedString);
	}

	// Update the phase to match with amountDestroyed.
	updateCurrentPhase(false);
}

void OneBlock::loadPhases()
{
	// Get the Resources folder path and add "Phases" to get the path to the Phases folder.
	std::wstring installFolderPath = GetThisModInstallFolderPath();
	std::wstring phasesFolderPath = installFolderPath.substr(0, installFolderPath.find(L"Update")) + L"Resources\\Phases"; // Use the install folder path to find the Resources folder.

	// Iterate through all of the files in the Phases folder and add the phases to the phases field.
	for (const auto& entry : std::filesystem::directory_iterator(phasesFolderPath)) {
		phases.push_back(Phase(entry.path()));
	}
}

void OneBlock::load()
{
	// Read file to check whether active world is a OneBlock world.
	std::wstring loadedString;
	if (LoadModDataString(L"OneBlock\\isOneBlock", loadedString)) {
		isOneBlock = std::stoi(loadedString);
	}
	else { // If such a file doesn't exist (should be the case for all worlds that aren't OneBlock).
		isOneBlock = false;
	}

	// If it was indeed a OneBlock world, then load phases and progress.
	if (isOneBlock) {

		// Check if first time world is loaded.
		LoadModDataString(L"OneBlock\\isFirstLoad", loadedString);
		if (std::stoi(loadedString)) {
			// Tell the player some stuff.
			printHintText(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, L"Welcome to OneBlock!\nIf there's ever anything you're unsure of,\ncheck the tutorial in the briefcase - the answer might be there.\nIf it isn't, then feel free to ask on the Workshop.\nTo begin, simply break the block underneath you.\nHave fun!", -1);
			SaveModDataString(L"OneBlock\\isFirstLoad", L"0");
		}

		loadPhases();
		loadProgress();

		// Do some checks, and fix the OneBlock if something is wrong.
		BlockInfo block = GetBlock(center);
		if (block.Type == EBlockType::Air) {// If somehow the OneBlock has disappeared, this will set it back.
			setOneBlock();
		}
		else if (block.Type == EBlockType::ModBlock && block.CustomBlockID != lootBlockID) { // If it's a ModBlock but isn't lootBlock, change it (happens at first load).
			ignoreBlockDestroyment = true;
			ignoreBlockPlacement = true;
			setOneBlock();
		}
	}
}

void OneBlock::askIfCreate(bool printText, CoordinateInCentimeters initialSpawn)
{
	// Print three hint texts for the player to interact with and save them in hintTextHandles to delete later.
	if (printText) {
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Do you want to create the\nOneBlock world?", 60));
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, -50, -20), L"Yes", 60));
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 50, -20), L"No", 60));
	}

	// Check if any of the player's hands are in the Yes or No hint texts.
	checkHand(true, initialSpawn);
	checkHand(false, initialSpawn);
}

void OneBlock::checkHand(bool leftHand, CoordinateInCentimeters initialSpawn)
{
	CoordinateInCentimeters handLoc = GetHandLocation(leftHand);

	// As soon as the player's hand is in any of the hint texts, stop asking
	// if the player wants to create the OneBlock world, destroy all hint texts,
	// and either create the OneBlock world or don't.
	if (isBetween(initialSpawn + CoordinateInCentimeters(30, -70, -40), initialSpawn + CoordinateInCentimeters(70, -30, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		askingIfCreate = false;
		destroyHintTexts();
		void* handle = SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Okay. The world is now being created.\nPlease wait.", -1);
		if (create()) { // Create OneBlock world.
			DestroyHintText(handle);
			SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"The world has now been created.\nGo ahead and leave this world\nand join it!", 10);
		} 
	}
	else if (isBetween(initialSpawn + CoordinateInCentimeters(30, 30, -40), initialSpawn + CoordinateInCentimeters(70, 70, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		askingIfCreate = false;
		destroyHintTexts();
		SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Alright. You will be asked again the\nnext time you join a world if\nyou change your mind.", 10);
	}
}

bool OneBlock::isBetween(CoordinateInCentimeters corner1, CoordinateInCentimeters corner2, CoordinateInCentimeters loc)
{
	return loc.X >= corner1.X && loc.X <= corner2.X && loc.Y >= corner1.Y && loc.Y <= corner2.Y && loc.Z >= corner1.Z && loc.Z <= corner2.Z;
}

void OneBlock::createVoidWorld()
{
	SetBlock(center, EBlockType::Grass);
	SetPlayerLocation(center + CoordinateInBlocks(0, 0, 1));
	SpawnHintText(center + CoordinateInBlocks(1, 0, 3), L"Please wait for everything\n to be removed.", 20);
	for (int i = -1248; i < 1248; i++) {
		for (int j = -1248; j < 1248; j++) {
			for (int k = 1; k < 800; k++) {
				SetBlock(CoordinateInBlocks(i, j, k), EBlockType::Air);
			}
		}
	}
	SetBlock(center, EBlockType::Grass);
	SetPlayerLocation(center + CoordinateInBlocks(0, 0, 1));
	SpawnHintText(center + CoordinateInBlocks(1, 0, 3), L"Everything has now been removed.\nGo ahead and begin!", 7.5);
}

void OneBlock::incrementAmount()
{
	amountDestroyed++;
	SaveModDataString(L"OneBlock\\amountDestroyed", std::to_wstring(amountDestroyed)); // Save progress.
	
	// Check whether the phase has changed.
	if (!currentPhase.isInPhase(amountDestroyed)) {
		updateCurrentPhase(true);
	}
}

void OneBlock::printAmountDestroyed()
{
	// Calculate the size of the hint text to be used according to the amount of digits in amountDestroyed.
	int digits = numDigits(amountDestroyed);
	float offset = 1.0F;

	for (int i = 0; i < digits - 2; i++) { // Offset for every new digit after 2 digits
		offset += 0.25F;
	}

	// Use the custom method to print the hint text.
	printHintText(getHintTextLocation(50, 40), std::to_wstring(amountDestroyed), 3, 0.3F * offset, 2.0F / offset, 10);
}

void OneBlock::setOneBlock()
{
	// Gets a random block from the possible blocks in the current phase,
	// and then sets it.
	BlockInfo blockToSet = currentPhase.getRandomBlock();
	if (blockToSet.Type != EBlockType::Invalid) {
		// If it's a grass block that is being set, sometimes set some foliage
		// on top of the block as well.
		if (blockToSet.Type == EBlockType::Grass) {
			int randomInt = GetRandomInt<0, 99>();
			ignoreBlockPlacement = true;
			// Hardcoded chances for foliage.
			if (randomInt < 2) { // 2% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::FlowerRainbow);
			}
			else if (randomInt < 7) { // 5% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::Flower1);
			}
			else if (randomInt < 12) { // 5% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::Flower2);
			}
			else if (randomInt < 17) { // 5% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::Flower3);
			}
			else if (randomInt < 22) { // 5% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::Flower4);
			}
			else if (randomInt < 37) { // 15% chance.
				SetBlock(center + CoordinateInBlocks(0, 0, 1), EBlockType::GrassFoliage);
			}
			else {
				ignoreBlockPlacement = false;
			}
			
		}
		SetBlock(center, blockToSet);
	}
}

void OneBlock::updateCurrentPhase(bool printPhase)
{
	// Goes through all of the possible phases and updates the current
	// phase to be the one it should be.

	for (Phase p : phases) {
		if (p.isInPhase(amountDestroyed)) {
			currentPhase = p;
			break;
		}
	}

	if (printPhase) {
		SpawnHintTextAdvanced(getHintTextLocation(80, 44), L"New phase!\nWelcome to the " + currentPhase.name + L" phase!\n" + currentPhase.description, 7, 1, 1.5F);
	}
}

bool OneBlock::exists()
{
	// Get the worlddata folder, check if there is a world with the name "OneBlock".
	
	std::wstring saveFolderPath = GetThisModSaveFolderPath(L"");
	std::wstring path = saveFolderPath.substr(0, saveFolderPath.find(L"WorldData") + 9);

	// Go through all entries in the path, meaning all the worlds in this case.
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		std::filesystem::path worldName = entry.path().filename();
		bool isOneBlockWorld = worldName == "OneBlock";
		if (isOneBlockWorld) {
			// Go through all entries in this path too, check if it is actually deleted
			for (const auto& subEntry : std::filesystem::directory_iterator(entry.path())) {
				std::filesystem::path folderName = subEntry.path().filename();
				bool isDeleted = folderName == "WorldIsDeleted";
				if (isDeleted) {
					return false;
				}
			}

			return true;
		}
	}

	return false;
}

bool OneBlock::create()
{
	// Copy the world from the resources folder to the WorldData folder.
	std::wstring installFolderPath = GetThisModInstallFolderPath();
	std::wstring saveFolderPath = GetThisModSaveFolderPath(L"");

	std::wstring oldPath = installFolderPath.substr(0, installFolderPath.find(L"Update")) + L"Resources\\TemplateWorld"; // Use the install folder path to find the Resources folder.
	std::wstring newPath = saveFolderPath.substr(0, saveFolderPath.find(L"WorldData") + 9) + L"\\OneBlock"; // Use the mod save folder path to find the WorldData folder.

	try {
		std::filesystem::remove_all(newPath);
		std::filesystem::copy(oldPath, newPath, std::filesystem::copy_options::recursive);
	}
	catch (std::filesystem::filesystem_error const& ex) {
		std::string message = ex.code().message();
		SpawnHintTextAdvanced(GetPlayerLocationHead() + GetPlayerViewDirection() * 50, L"Something went wrong when making the OneBlock world:\n" + std::wstring(message.begin(), message.end()), 10);
		return false;
	}

	return true;
}

void OneBlock::destroyHintTexts()
{
	// Simply go through all of the hint text handles and destroy them.

	auto it = hintTextHandles.begin();
	while (it != hintTextHandles.end()) {
		DestroyHintText(*it);
		it = hintTextHandles.erase(it);
	}
}

void OneBlock::printHintText(CoordinateInCentimeters location, std::wstring text, float duration, float sizeMul, float sizeMulVer, float fontMul)
{
	// Remove the old hint text with the saved handle, and spawn the
	// new one as well as save its handle.

	DestroyHintText(currentHintTextHandle);
	currentHintTextHandle = SpawnHintTextAdvanced(location, text, duration, sizeMul, sizeMulVer, fontMul);
}

bool OneBlock::isOutOfBounds(CoordinateInBlocks location)
{
	return location.Z < 10 || location.Z > 810 || location.X < -600 || location.X > 600 || location.Y < -600 || location.Y > 600;
}

int OneBlock::numDigits(int number)
{
	int digits = 0;
	while (number) {
		number /= 10;
		digits++;
	}
	return digits;
}

void OneBlock::giveLoot()
{
	// Get some random loot from the current phase, and then
	// give it to the player. Also spawn a hint text that
	// tells the player what they got.

	// Get the random loot.
	std::vector<Loot> loot = currentPhase.getRandomLoot();

	std::wstring message = L"You have been given:";

	// Add loot to player's inventory and append to the message.
	for (Loot l : loot) {
		AddToInventory(l.type, l.amount);
		message = message + L"\n" + std::to_wstring(l.amount) + L" ";
		message = message + (l.amount > 1 ? getPluralWString(getLootWStringFromBlockInfo(l.type)) : getLootWStringFromBlockInfo(l.type));
	}
	// Calculate the vertical size of the hint text.
	int differentLoot = (int) loot.size();
	int sizeMulVer = differentLoot - 2 < 1 ? 0 : differentLoot - 2;
	
	// Spawn hint text.
	SpawnHintTextAdvanced(getHintTextLocation(80, 44), message, 5, 1, 1 + sizeMulVer * 0.3F);
}

CoordinateInCentimeters OneBlock::getHintTextLocation(int height, int radius)
{
	CoordinateInCentimeters loc = center + CoordinateInCentimeters(0, 0, height); // The location to use for the hint text that is spawned - this being the default.

	// Calculate where on a circle with the formula x^2+y^2=radius^2 to spawn the hint text,
	// with the default vector being the location of the player relative to the center - 
	// but if on top of the center block, use the direction the player is looking instead
	// because it can look pretty funky otherwise.
	CoordinateInCentimeters playerLoc = GetPlayerLocationHead();
	CoordinateInCentimeters directionVector = CoordinateInCentimeters(center) - playerLoc; // Default vector to use for calculation.

	if (playerLoc.X >= -25 && playerLoc.X <= 25 && playerLoc.Y >= -25 && playerLoc.Y <= 25) {
		directionVector = CoordinateInCentimeters(GetPlayerViewDirection() * 100); // Vector to use if on top of center block.
	}

	// Variables for the quadratic equation.
	int64_t a = directionVector.X * directionVector.X + directionVector.Y * directionVector.Y;
	int64_t b = 2 * (playerLoc.X * directionVector.X + playerLoc.Y * directionVector.Y);
	int64_t c = playerLoc.X * playerLoc.X + playerLoc.Y * playerLoc.Y - (radius * radius);

	int64_t d = b * b - 4 * a * c; // The discriminant.

	if (d > 0) { // Two intersections.
		double t1 = (-b + sqrt(d)) / (2 * a);
		double t2 = (-b - sqrt(d)) / (2 * a);
		double t = 0;
		if (playerLoc.Z > CoordinateInCentimeters(center).Z + 25) { // If the player's head is above the center block, use the opposite intersection to spawn the hint text at. Otherwise, the hint text will be in the way of the block.
			t = std::max(t1, t2);
		}
		else { // Else, use the first one. Otherwise, the block will be in the way of the hint text.
			t = std::min(t1, t2);
			loc.Z = loc.Z - 20;
		}
		double x = playerLoc.X + t * directionVector.X;
		double y = playerLoc.Y + t * directionVector.Y;
		loc.X = (int)round(x);
		loc.Y = (int)round(y);
	}
	else if (d == 0) { // One intersection - just use the point found. Should technically never happen.
		double t = (-b) / (2.0 * a);
		double x = playerLoc.X + t * directionVector.X;
		double y = playerLoc.Y + t * directionVector.Y;
		loc.X = (int)round(x);
		loc.Y = (int)round(y);
	}
	// No intersections can also technically never happen because either the player is outside the block,
	// where the center is then used to calculate the vector to use (meaning there will always be two intersections),
	// or the player is inside the block, where the direction the player is looking is used as the vector,
	// meaning there will always be an intersection because the player is always looking out at the circle.
	
	return loc;
}

void OneBlock::removeDrops()
{
	drops = ConsumeBlockItems(center + CoordinateInBlocks(0, 0, 2), std::vector<BlockInfo>(), 0, CoordinateInCentimeters(25, 25, 100), 50);
}

void OneBlock::spawnCustomDrops()
{
	int numOfDropsToRemove = 0;
	BlockInfo typeOfDropsToRemove;
	switch (previousBlock.Type) {
	case EBlockType::TreeWood:
		numOfDropsToRemove = 5;
		typeOfDropsToRemove = getNativeDropFromBlockInfo(EBlockType::TreeWood);
		drops.push_back(BlockInfoWithLocation{ EBlockType::TreeWood });
		break;
	case EBlockType::TreeWoodBright:
		numOfDropsToRemove = 5;
		typeOfDropsToRemove = getNativeDropFromBlockInfo(EBlockType::TreeWoodBright);
		drops.push_back(BlockInfoWithLocation{ EBlockType::TreeWoodBright });
		break;
	}

	auto it = drops.begin();
	while (it != drops.end()) {
		if (numOfDropsToRemove > 0) {
			if (it->Info.Type == typeOfDropsToRemove.Type) {
				it = drops.erase(it);
			}
			else {
				++it;
			}
		}
		else {
			break;
		}
	}

	// Get all existing drops.
	std::vector<BlockInfoWithLocation> existingDrops = {};

	// Spawn all of the drops.
	CoordinateInCentimeters offset = CoordinateInCentimeters(0, 0, 31); // The offset from the center where block items are spawned.
	
	// All possible locations.
	std::vector<PossibleLocation> possibleLocations = {
		PossibleLocation(CoordinateInCentimeters(0, 0, 0), true),
		PossibleLocation(CoordinateInCentimeters(16, 0, 0), true),
		PossibleLocation(CoordinateInCentimeters(0, 16, 0), true),
		PossibleLocation(CoordinateInCentimeters(-16, 0, 0), true),
		PossibleLocation(CoordinateInCentimeters(0, -16, 0), true),
		PossibleLocation(CoordinateInCentimeters(16, 16, 0), true),
		PossibleLocation(CoordinateInCentimeters(-16, 16, 0), true),
		PossibleLocation(CoordinateInCentimeters(-16, -16, 0), true),
		PossibleLocation(CoordinateInCentimeters(16, -16, 0), true)
	};
	bool doDoubleBreak = false; // Used to break out of both for loops.
	CoordinateInCentimeters centerCm = CoordinateInCentimeters(center);
	for (BlockInfoWithLocation drop : drops) {
		
		for (int i = centerCm.Z + offset.Z; i <= centerCm.Z + offset.Z + 250; i += 13) {
			for (PossibleLocation &p : possibleLocations) {
				p.location.Z = i;
				p.possible = true;
			}
			for (const BlockInfoWithLocation &e : existingDrops) {
				for (PossibleLocation &p : possibleLocations) {
					if (p.location.X + 4 >= e.Location.X && p.location.X - 4 <= e.Location.X &&
						p.location.Y + 4 >= e.Location.Y && p.location.Y - 4 <= e.Location.Y &&
						p.location.Z + 4 >= e.Location.Z && p.location.Z - 4 <= e.Location.Z) {
						p.possible = false;
					}
				}
			}
			for (const PossibleLocation &p : possibleLocations) {
				if (p.possible) {
					SpawnBlockItem(p.location, drop.Info.Type);
					existingDrops.push_back(BlockInfoWithLocation{ drop.Info.Type, p.location });
					doDoubleBreak = true;
					break;
				}
			}
			if (doDoubleBreak) {
				doDoubleBreak = false;
				break;
			}
		}
	}
}