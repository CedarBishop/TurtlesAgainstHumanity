// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPingPongComponent.h"

// Sets default values for this component's properties
UCustomPingPongComponent::UCustomPingPongComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InitializeVariables();
}


// Called when the game starts
void UCustomPingPongComponent::BeginPlay()
{
	Super::BeginPlay();
	pingPongTime = FMath::FRandRange(minPingPongTime,maxPingPongTime);
	FVector newScale = FVector::OneVector * (FMath::FRandRange(minScale, maxScale));
	SetRelativeScale3D(newScale);
	timer = pingPongTime;
	
}


// Called every frame
void UCustomPingPongComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector addedVector = (bIsGoingRight)?  FVector(-0.2f,0.4f,0.0f) : FVector(0.2f,-0.4f,0.0f);

	AddRelativeLocation(addedVector);

	if (timer <= 0)
	{
		bIsGoingRight = !bIsGoingRight;
		timer = pingPongTime;
	}
	else
	{
		timer -= DeltaTime;
	}
}

void UCustomPingPongComponent::InitializeVariables()
{
	 minScale = 0.5f;
	 maxScale = 2.0f;
	 minPingPongTime = 0.0f;
	 maxPingPongTime = 5.0f;

}

