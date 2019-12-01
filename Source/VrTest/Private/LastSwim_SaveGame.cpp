// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwim_SaveGame.h"

ULastSwim_SaveGame::ULastSwim_SaveGame()
{
	saveGameSlotName = TEXT("SlotOne");
	userIndex = 0;

	sfxVolume = 1.0f;
	bgmVolume = 1.0f;
	LODLevel = 0;
}