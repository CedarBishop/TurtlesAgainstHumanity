// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwimGameMode_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Base.h"
#include "LastSwim_SaveGame.h"
#include "LastSwim_GameInstance.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"

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
	if (gameInstance == NULL)
	{
		gameInstance = Cast<ULastSwim_GameInstance>(UGameplayStatics::GetGameInstance(this));
		gameInstance->gamemode = this;
	}
	playerLastPosition = player->GetActorLocation();
	delayTimer = 0.1f;


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
	if (delayTimer <= 0 && bHasLoadedSettings == false)
	{
		LoadSettings();
		bHasLoadedSettings = true;
	}
	else
	{
		delayTimer -= DeltaSeconds;
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
			SaveGameInstance->bHasBeenSaved = true;
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Saved Values"));
			}*/
		}



		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, slotName, userIndex))
		{
			/*if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Saved Game"));
			}*/
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
			if (LoadedGame->bHasBeenSaved == true)
			{
				gameInstance->sfxVolume = LoadedGame->sfxVolume;
				gameInstance->musicVolume = LoadedGame->bgmVolume;
				gameInstance->LODLevel = LoadedGame->LODLevel;

				/*if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loaded Saved Values"));
				}*/
			}
			else
			{
				gameInstance->sfxVolume = 1.0f;
				gameInstance->musicVolume = 1.0f;
				gameInstance->LODLevel = 1;

				//if (GEngine)
				//{
				//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loaded Default Values"));
				//}
			}
			
			
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
	else
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loaded Game Failed"));
		}*/
	}
}

