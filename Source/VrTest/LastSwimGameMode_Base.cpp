// Fill out your copyright notice in the Description page of Project Settings.


#include "LastSwimGameMode_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Player_Base.h"


ALastSwimGameMode_Base::ALastSwimGameMode_Base()
{
	if (player == nullptr)
	{
		player = Cast<APlayer_Base>(UGameplayStatics::GetPlayerPawn(this, 0));
	}
	delayBetweenChecks = 0;
	minSpawnObstacles = 3;
	maxSpawnObstacles = 5;


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

