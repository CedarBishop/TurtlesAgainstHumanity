// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwim_GameInstance.h"

void ULastSwim_GameInstance::Init()
{
	Super::Init();

	bFirstPlaythrough = true;
	musicVolume = 1.0f;
	sfxVolume = 1.0f;
	LODLevel = 2;
}