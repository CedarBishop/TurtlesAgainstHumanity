// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LastSwim_GameInstance.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class VRTEST_API ULastSwim_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	virtual void Init() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFirstPlaythrough;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float musicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sfxVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LODLevel;
};
