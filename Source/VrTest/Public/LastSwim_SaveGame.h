// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LastSwim_SaveGame.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VRTEST_API ULastSwim_SaveGame : public USaveGame
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game ID Vars")
	FString saveGameSlotName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save Game ID Vars")
	int userIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float sfxVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float bgmVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int LODLevel;
};
