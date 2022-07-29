#include "OneBlock.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <math.h>

bool OneBlock::askingIfCreate = false;
std::vector<void*> OneBlock::hintTextHandles;

OneBlock::OneBlock()
{
	this->isLoaded = false;
	this->isOneBlock = false;
	this->center = CoordinateInBlocks(0, 0, 200);
	this->amountDestroyed = 0;
	this->phase = 1;
	this->currentHintTextHandle = nullptr;
}

void OneBlock::loadProgress()
{
	std::wstring loadedString;
	if (LoadModDataString(L"OneBlock\\amountDestroyed", loadedString)) {
		amountDestroyed = std::stoi(loadedString);
	}
	updatePhase();
}

void OneBlock::load()
{
	std::wstring loadedString;
	if (LoadModDataString(L"OneBlock\\isOneBlock", loadedString)) {
		isOneBlock = std::stoi(loadedString);
	}
	else { // If such a file doesn't exist (should be the case for all worlds that aren't OneBlock)
		isOneBlock = false;
	}

	if (isOneBlock) {
		loadProgress();
	}
	isLoaded = true;
}

void OneBlock::askIfCreate(bool printText, CoordinateInCentimeters initialSpawn)
{
	if (printText) {
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Do you want to create the\nOneBlock world?", 60));
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, -50, -20), L"Yes", 60));
		hintTextHandles.push_back(SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 50, -20), L"No", 60));
	}

	checkHand(true, initialSpawn);
	checkHand(false, initialSpawn);
}

void OneBlock::checkHand(bool leftHand, CoordinateInCentimeters initialSpawn)
{
	CoordinateInCentimeters handLoc = GetHandLocation(leftHand);

	if (isBetween(initialSpawn + CoordinateInCentimeters(30, -70, -40), initialSpawn + CoordinateInCentimeters(70, -30, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		askingIfCreate = false;
		destroyHintTexts();
		void* handle = SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Okay. The world is now being created.\nPlease wait.", -1);
		create();
		DestroyHintText(handle);
		SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"The world has now been created.\nGo ahead and leave this world\nand join it!", 10);
	}
	else if (isBetween(initialSpawn + CoordinateInCentimeters(30, 30, -40), initialSpawn + CoordinateInCentimeters(70, 70, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		askingIfCreate = false;
		destroyHintTexts();
		SpawnHintTextAdvanced(initialSpawn + CoordinateInCentimeters(50, 0, 0), L"Alright. You will be asked again the\nnext time you join a world if\n you change your mind.", 10);
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
	SaveModDataString(L"OneBlock\\amountDestroyed", std::to_wstring(amountDestroyed));
	updatePhase();
}

void OneBlock::printAmountDestroyed()
{
	CoordinateInCentimeters loc = center + CoordinateInBlocks(0, 0, 1);

	CoordinateInCentimeters playerLoc = GetPlayerLocationHead();
	CoordinateInCentimeters directionVector = CoordinateInCentimeters(center) - playerLoc;

	if (playerLoc.X >= -25 && playerLoc.X <= 25 && playerLoc.Y >= -25 && playerLoc.Y <= 25) {
		directionVector = CoordinateInCentimeters(GetPlayerViewDirection()*1000);
	}

	float a = directionVector.X * directionVector.X + directionVector.Y * directionVector.Y;
	float b = 2 * (playerLoc.X * directionVector.X + playerLoc.Y * directionVector.Y);
	float c = playerLoc.X * playerLoc.X + playerLoc.Y * playerLoc.Y - 1250;

	float d = b * b - 4 * a * c;

	if (d > 0) {
		float t1 = (-b + sqrt(d)) / (2 * a);
		float t2 = (-b - sqrt(d)) / (2 * a);
		float t = 0;
		if (playerLoc.Z > CoordinateInCentimeters(center).Z + 25) {
			t = std::max(t1, t2);
		}
		else {
			t = std::min(t1, t2);
		}
		float x = playerLoc.X + t * directionVector.X;
		float y = playerLoc.Y + t * directionVector.Y;
		loc.X = round(x);
		loc.Y = round(y);
	}
	else if (d == 0) {
		float t = (-b) / (2 * a);
		float x = playerLoc.X + t * directionVector.X;
		float y = playerLoc.Y + t * directionVector.Y;
		loc.X = round(x);
		loc.Y = round(y);
	}
	
	printHintText(loc, std::to_wstring(amountDestroyed), 3, 0.8, 2);
}

void OneBlock::setOneBlock()
{
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
	// Copy the world from the install folder to the WorldData folder
	std::wstring oldPath = GetThisModInstallFolderPath() + L"TemplateWorld";
	std::wstring saveFolderPath = GetThisModSaveFolderPath(L"");

	std::wstring newPath = saveFolderPath.substr(0, saveFolderPath.find(L"WorldData")+9) + L"\\OneBlock";

	std::filesystem::copy(oldPath, newPath, std::filesystem::copy_options::recursive);

	// Write to the global save file that a OneBlock world now exists
	std::wstring globalSaveFolderPath = GetThisModGlobalSaveFolderPath(L"OneBlock");
	std::ofstream file(globalSaveFolderPath + L"exists.txt");
	file << "1";
	file.close();
}

void OneBlock::destroyHintTexts()
{
	auto it = hintTextHandles.begin();
	while (it != hintTextHandles.end()) {
		DestroyHintText(*it);
		it = hintTextHandles.erase(it);
	}
}

void OneBlock::printHintText(CoordinateInCentimeters location, std::wstring text, float duration, float sizeMul, float sizeMulVer)
{
	DestroyHintText(currentHintTextHandle);
	currentHintTextHandle = SpawnHintTextAdvanced(location, text, duration, sizeMul, sizeMulVer);
}