// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceToObjectsDetector.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UDistanceToObjectsDetector::UDistanceToObjectsDetector()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	tagOfObjects = "Coral";
	distanceToReact = 1000.0f;
	// ...
}


// Called when the game starts
void UDistanceToObjectsDetector::BeginPlay()
{
	Super::BeginPlay();

	SetActorList();
	
}


// Called every frame
void UDistanceToObjectsDetector::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDistanceToObjectsDetector::SetActorList()
{
	if (tagOfObjects != "")
	{
		setActorArrayCount++;
		UGameplayStatics::GetAllActorsWithTag(this, tagOfObjects, allOfObject);
	}
}

AActor* UDistanceToObjectsDetector::GetClosestObject()
{
	if (allOfObject.Num())
	{
		return NULL;
	}
	FVector currentLocation = GetOwner()->GetActorLocation();
	float lowestDistance = 1000000.0f;
	int potentialTargetIndex = 0;

	for (int i = 0; i < allOfObject.Num(); i++)
	{
		float thisDistance = FVector::Distance(currentLocation, allOfObject[i]->GetActorLocation());
		if (thisDistance < distanceToReact)
		{
			if (bFoundObject)
			{
				if (thisDistance < lowestDistance)
				{
					lowestDistance = thisDistance;
					potentialTargetIndex = i;
				}
			}
			else
			{
				bFoundObject = true;
				lowestDistance = thisDistance;
				potentialTargetIndex = i;
			}
		}
	}

	if (bFoundObject)
	{
		AActor* potentialTarget = allOfObject[potentialTargetIndex];
		return potentialTarget;
	}
	return NULL;
}

