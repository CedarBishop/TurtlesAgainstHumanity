// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DistanceToObjectsDetector.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEST_API UDistanceToObjectsDetector : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDistanceToObjectsDetector();


private:

	bool bFoundObject;

	void SetActorList();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modularize Tool")
	FName tagOfObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Modularize Tool")
	float distanceToReact;

	UFUNCTION(BlueprintCallable)
	AActor* GetClosestObject();

	TArray<AActor*> allOfObject;
	int setActorArrayCount;

};
