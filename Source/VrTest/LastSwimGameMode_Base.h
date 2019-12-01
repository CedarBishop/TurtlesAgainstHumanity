// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LastSwimGameMode_Base.generated.h"


USTRUCT(BlueprintType)
struct FDistanceEvents
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int eventNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float distanceRequired;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasBeenCalled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFirstPlaythrough;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString breakingNewsText;
};



/**
 * 
 */
UCLASS(Blueprintable)
class VRTEST_API ALastSwimGameMode_Base : public AGameMode
{
	GENERATED_BODY()
	

private:

	// Non Blueprint Functions

	void CheckDistanceTravelled();
	
	void CheckEvents();


	//Non Blueprint variables

	float timer;


public:
	// Sets default values for this pawn's properties
	ALastSwimGameMode_Base();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Blueprintable Functions

	UFUNCTION(BlueprintNativeEvent)
	void ExecuteCallEvent();

	UFUNCTION(BlueprintCallable)
	FVector GetRandomLocationInRadius(float radius, FVector origin, float height);

	UFUNCTION(BlueprintCallable)
	void LoadSettings();

	UFUNCTION(BlueprintCallable)
	void SaveSettings();

	// Blueprintable Components






	// Blueprintable Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDistanceEvents> distanceEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float distanceTravelled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector playerLastPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFirstPlaythough;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayer_Base* player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float delayBetweenChecks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int minSpawnObstacles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxSpawnObstacles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxSpawnDistanceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int currentEventNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString currentBreakingNewsText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game Vars")
	FString slotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game Vars")
	int userIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULastSwim_GameInstance* gameInstance;

};