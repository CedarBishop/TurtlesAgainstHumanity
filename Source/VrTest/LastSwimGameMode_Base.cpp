// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwimGameMode_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Base.h"
#include "LastSwim_SaveGame.h"
#include "LastSwim_GameInstance.h"


ALastSwimGameMode_Base::ALastSwimGameMode_Base()
{
	if (player == nullptr)
	{
		player = Cast<APlayer_Base>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	delayBetweenChecks = 0;
	minSpawnObstacles = 3;
	maxSpawnObstacles = 5;

	slotName = "SlotOne";
	userIndex = 0;


}


void ALastSwimGameMode_Base::BeginPlay()
{
	Super::BeginPlay();

	if (player == nullptr)
	{
		player = Cast<APlayer_Base>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	playerLastPosition = player->GetActorLocation();

}

void ALastSwimGameMode_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (timer <= 0)
	{
		timer = delayBetweenChecks;
		CheckDistanceTravelled();
		CheckEvents();
	}
	else
	{
		timer -= DeltaSeconds;
	}
}

void ALastSwimGameMode_Base::ExecuteCallEvent_Implementation()
{
	// do something
}

void ALastSwimGameMode_Base::CheckDistanceTravelled()
{
	if (player == nullptr)
	{
		player = Cast<APlayer_Base>(UGameplayStatics::GetPlayerPawn(this,0));		
	}
	distanceTravelled += (player->GetActorLocation() - playerLastPosition).Size();
	playerLastPosition = player->GetActorLocation();
}

void ALastSwimGameMode_Base::CheckEvents()
{
	for (int i = 0; i < distanceEvents.Num(); i++)
	{
		if (!distanceEvents[i].bHasBeenCalled)
		{
					UE_LOG(LogTemp, Warning, TEXT("Ran call events"));
			if (bIsFirstPlaythough == distanceEvents[i].bFirstPlaythrough)
			{
				if (distanceTravelled >= distanceEvents[i].distanceRequired)
				{
					currentEventNumber = distanceEvents[i].eventNumber;
					currentBreakingNewsText = distanceEvents[i].breakingNewsText;
					ExecuteCallEvent();
					distanceEvents[i].bHasBeenCalled = true;
				}
			}
		}
	}
}

FVector ALastSwimGameMode_Base::GetRandomLocationInRadius(float radius, FVector origin, float height)
{
	FVector newLocation = FVector(FMath::FRandRange(origin.X - radius, origin.X + radius),FMath::FRandRange(origin.Y - radius, origin.Y + radius), height);
	return newLocation;
}

void ALastSwimGameMode_Base::SaveSettings()
{
	if (ULastSwim_SaveGame* SaveGameInstance = Cast<ULastSwim_SaveGame>(UGameplayStatics::CreateSaveGameObject(ULastSwim_SaveGame::StaticClass())))
	{
		if (gameInstance == NULL)
		{
			gameInstance = Cast<ULastSwim_GameInstance>(UGameplayStatics::GetGameInstance(this));
		}

		if (gameInstance != NULL)
		{
			// Set data on the savegame object.
			SaveGameInstance->LODLevel = gameInstance->LODLevel;
			SaveGameInstance->sfxVolume = gameInstance->sfxVolume;
			SaveGameInstance->bgmVolume = gameInstance->musicVolume;
		}



		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, slotName, userIndex))
		{
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, slotName, userIndex);
			// Save succeeded.
		}
	}
}

void ALastSwimGameMode_Base::LoadSettings()
{
	if (ULastSwim_SaveGame* LoadedGame = Cast<ULastSwim_SaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, userIndex)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		//Set game instance variables with ones from loadedgame
		if (gameInstance == NULL)
		{
			gameInstance = Cast<ULastSwim_GameInstance>(UGameplayStatics::GetGameInstance(this));
		}

		if (gameInstance != NULL)
		{
			gameInstance->sfxVolume = LoadedGame->sfxVolume;
			gameInstance->musicVolume = LoadedGame->bgmVolume;
			gameInstance->LODLevel = LoadedGame->LODLevel;
		}

		if (player == NULL)
		{
			player = Cast <APlayer_Base>(UGameplayStatics::GetPlayerPawn(this, 0));			
		}

		if (player != NULL)
		{
			player->SetSFXVolume(LoadedGame->sfxVolume);
			player->SetMusicVolume(LoadedGame->bgmVolume);
			// set material quality as well here
		}

	}
}

