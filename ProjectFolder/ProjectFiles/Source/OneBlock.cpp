#include "OneBlock.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <math.h>

bool OneBlock::askingIfCreate = false;
std::vector<void*> OneBlock::hintTextHandles;

OneBlock::OneBlock()
{
	// All default values for the fields.
	this->isOneBlock = false;
	this->center = CoordinateInBlocks(0, 0, 200);
	this->amountDestroyed = 0;
	this->phase = 1;
	this->currentHintTextHandle = nullptr;
}

void OneBlock::loadProgress()
{
	// Get amountDestroyed from the world save folder.
	std::wstring loadedString;
	if (LoadModDataString(L"OneBlock\\amountDestroyed", loadedString)) {
		amountDestroyed = std::stoi(loadedString);
	}

	// Update the phase to match with amountDestroyed.
	updatePhase();
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

	// If it was indeed a OneBlock world, then load progress.
	if (isOneBlock) {
		loadProgress();
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
		create(); // Create OneBlock world.
		DestroyHintText(handle);
		SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"The world has now been created.\nGo ahead and leave this world\nand join it!", 10);
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
	updatePhase(); // Update the phase if necessary.
}

void OneBlock::printAmountDestroyed()
{
	CoordinateInCentimeters loc = center + CoordinateInBlocks(0, 0, 1); // The location to use for the hint text that is spawned, this being the default.

	// Calculate where on a circle with the formula x^2+y^2=1250 to spawn the hint text,
	// with the default vector being the location of the player relative to the center - 
	// but if on top of the center block, use the direction the player is looking instead
	// because it can look pretty funky otherwise.
	CoordinateInCentimeters playerLoc = GetPlayerLocationHead();
	CoordinateInCentimeters directionVector = CoordinateInCentimeters(center) - playerLoc; // Default vector to use for calculation.

	if (playerLoc.X >= -25 && playerLoc.X <= 25 && playerLoc.Y >= -25 && playerLoc.Y <= 25) {
		directionVector = CoordinateInCentimeters(GetPlayerViewDirection() * 1000); // Vector to use if on top of center block.
	}

	// Variables for the quadratic equation. Possibly no need for float.
	float a = directionVector.X * directionVector.X + directionVector.Y * directionVector.Y;
	float b = 2 * (playerLoc.X * directionVector.X + playerLoc.Y * directionVector.Y);
	float c = playerLoc.X * playerLoc.X + playerLoc.Y * playerLoc.Y - 1250;

	float d = b * b - 4 * a * c; // The discriminant.

	if (d > 0) { // Two intersections.
		float t1 = (-b + sqrt(d)) / (2 * a);
		float t2 = (-b - sqrt(d)) / (2 * a);
		float t = 0;
		if (playerLoc.Z > CoordinateInCentimeters(center).Z + 25) { // If the player's head is above the center block, use the opposite intersection to spawn the hint text at. Otherwise, the hint text will be in the way of the block.
			t = std::max(t1, t2);
		}
		else { // Else, use the first one. Otherwise, the block will be in the way of the hint text.
			t = std::min(t1, t2);
		}
		float x = playerLoc.X + t * directionVector.X;
		float y = playerLoc.Y + t * directionVector.Y;
		loc.X = round(x);
		loc.Y = round(y);
	}
	else if (d == 0) { // One intersection - just use the point found. Should technically never happen.
		float t = (-b) / (2 * a);
		float x = playerLoc.X + t * directionVector.X;
		float y = playerLoc.Y + t * directionVector.Y;
		loc.X = round(x);
		loc.Y = round(y);
	}
	// No intersections can also technically never happen because either the player is outside the block,
	// where the center is then used to calculate the vector to use (meaning there will always be two intersections),
	// or the player is inside the block, where the direction the player is looking is used as the vector,
	// meaning there will always be an intersection because the player is always looking out at the circle.

	// Use the custom method to print the hint text.
	printHintText(loc, std::to_wstring(amountDestroyed), 3, 0.5, 2, 4);
}

void OneBlock::setOneBlock()
{
	// TODO - Placeholder phases for the time being.
	switch (phase)
	{
	case 1:
		SetBlock(center, EBlockType::Grass);
		break;
	case 2:
		SetBlock(center, EBlockType::TreeWood);
		break;
	case 3:
		SetBlock(center, EBlockType::Stone);
		break;
	default:
		break;
	}
}

void OneBlock::updatePhase()
{
	// TODO - Placeholder phases for the time being.
	if (amountDestroyed < 3) {
		phase = 1;
	}
	else if (amountDestroyed < 10) {
		phase = 2;
	}
	else {
		phase = 3;
	}
}

bool OneBlock::exists()
{
	// Get the path to the folder where the data is stored and read the
	// exists.txt file there to see whether the OneBlock world already
	// exists. Simple stuff.
	std::wstring path = GetThisModGlobalSaveFolderPath(L"OneBlock");

	std::string resString;
	bool res = false;

	std::ifstream file(path + L"exists.txt");

	while (std::getline(file, resString)) {
		res = std::stoi(resString);
	}

	file.close();

	return res;
}

void OneBlock::create()
{
	// Copy the world from the install folder to the WorldData folder.
	std::wstring oldPath = GetThisModInstallFolderPath() + L"TemplateWorld";
	std::wstring saveFolderPath = GetThisModSaveFolderPath(L"");

	std::wstring newPath = saveFolderPath.substr(0, saveFolderPath.find(L"WorldData") + 9) + L"\\OneBlock"; // Use the mod save folder path to find the WorldData folder.

	std::filesystem::copy(oldPath, newPath, std::filesystem::copy_options::recursive);

	// Write to the global save file that a OneBlock world now exists.
	std::wstring globalSaveFolderPath = GetThisModGlobalSaveFolderPath(L"OneBlock");
	std::ofstream file(globalSaveFolderPath + L"exists.txt");
	file << "1";
	file.close();
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
	return location.Z < 10 || location.X < -600 || location.X > 600 || location.Y < -600 || location.Y > 600;
}
