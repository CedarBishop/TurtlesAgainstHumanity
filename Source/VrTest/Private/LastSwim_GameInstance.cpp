// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwim_GameInstance.h"
#include "LastSwimGameMode_Base.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"

void ULastSwim_GameInstance::Init()
{
	Super::Init();

	bFirstPlaythrough = true;
	//if (ALastSwimGameMode_Base* gamemode = Cast<ALastSwimGameMode_Base>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	gamemode->LoadSettings();
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ran Function"));
	//	}
	//}
	//else
	//{
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Did not Ran Function"));
	//	}
	//}
}