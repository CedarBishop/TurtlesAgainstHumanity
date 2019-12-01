// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwim_GameInstance.h"
#include "LastSwimGameMode_Base.h"
#include "Kismet/GameplayStatics.h"


void ULastSwim_GameInstance::Init()
{
	Super::Init();

	bFirstPlaythrough = true;
	if (ALastSwimGameMode_Base* gamemode = Cast<ALastSwimGameMode_Base>(  UGameplayStatics::GetGameMode(this)))
	{
		gamemode->LoadSettings();
	}
}