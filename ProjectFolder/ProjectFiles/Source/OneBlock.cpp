#include "OneBlock.h"

OneBlock::OneBlock()
{
	this->isLoaded = false;
	this->isOneBlock = false;
	this->center = CoordinateInBlocks(0, 0, 200);
	this->amountDestroyed = 0;
	this->phase = 1;
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

void OneBlock::askIfOneBlock(bool printText, CoordinateInCentimeters initialSpawn)
{
	if (printText) {
		SpawnHintText(initialSpawn + CoordinateInCentimeters(50, 0, -25), L"Do you want this world to\nbe a OneBlock world?", 60);
		SpawnHintText(initialSpawn + CoordinateInCentimeters(50, -50, -50), L"Yes", 60);
		SpawnHintText(initialSpawn + CoordinateInCentimeters(50, 50, -50), L"No", 60);
	}

	checkHand(true, initialSpawn);
	checkHand(false, initialSpawn);
}

void OneBlock::checkHand(bool leftHand, CoordinateInCentimeters initialSpawn)
{
	CoordinateInCentimeters handLoc = GetHandLocation(leftHand);

	if (isBetween(initialSpawn + CoordinateInCentimeters(25, -75, -50), initialSpawn + CoordinateInCentimeters(75, -25, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		createOneBlock();
		isOneBlock = true;
		isLoaded = true;
		SaveModDataString(L"OneBlock\\isOneBlock", L"1");
	}
	else if (isBetween(initialSpawn + CoordinateInCentimeters(25, 25, -50), initialSpawn + CoordinateInCentimeters(75, 75, 0), handLoc)) {
		PlayHapticFeedbackOnHand(leftHand, 0.1, 1, 1);
		SpawnHintText(initialSpawn + CoordinateInCentimeters(50, 50, -25), L"Choice confirmed! These popups will\ndisappear in under a minute.", 10);
		isOneBlock = false;
		isLoaded = true;
		SaveModDataString(L"OneBlock\\isOneBlock", L"0");
	}
}

bool OneBlock::isBetween(CoordinateInCentimeters corner1, CoordinateInCentimeters corner2, CoordinateInCentimeters loc)
{
	return loc.X > corner1.X && loc.X < corner2.X && loc.Y > corner1.Y && loc.Y < corner2.Y && loc.Z > corner1.Z && loc.Z < corner2.Z;
}

void OneBlock::createOneBlock()
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
	SpawnHintText(center + CoordinateInBlocks(1, 0, 3), L"Amount of blocks destroyed:\n" + std::to_wstring(amountDestroyed), 5);
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
