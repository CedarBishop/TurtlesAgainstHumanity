// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player_Base.generated.h"

UCLASS(Blueprintable)
class VRTEST_API APlayer_Base : public APawn
{
	GENERATED_BODY()

private:

	//private functions

	bool LowerOxygen();

	void InitializeComponents();

	void InitializeVariables();
	//

	//private variables
	float deltaTime;

	bool canLeftStroke;

	bool canRightStroke;

	//

public:
	// Sets default values for this pawn's properties
	APlayer_Base();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Functions Section

	UFUNCTION(BlueprintCallable)
	void ForwardStroke(float force);

	UFUNCTION(BlueprintCallable)
	void RaiseMinimumStressLevel(float addedStress);

	UFUNCTION(BlueprintCallable)
	void OxygenResupply(float stressHealed, float oxygenReplenished);

	UFUNCTION(BlueprintCallable)
	void ObstacleHit(float stressDamage, float oxygenDamage);

	UFUNCTION(BlueprintCallable)
	void StressRecovery();

	UFUNCTION(BlueprintCallable)
	void Brake();

	UFUNCTION(BlueprintCallable)
	void UnBrake();

	UFUNCTION(BlueprintCallable)
	void StartPcLeftCounter();

	UFUNCTION(BlueprintCallable)
	void StartPcRightCounter();

	UFUNCTION(BlueprintCallable)
	void EndPcLeftCounter();

	UFUNCTION(BlueprintCallable)
	void EndPcRightCounter();

	UFUNCTION(BlueprintCallable)
	void StartVrLeftCounter();

	UFUNCTION(BlueprintCallable)
	void StartVrRightCounter();

	UFUNCTION(BlueprintCallable)
	void EndVrLeftCounter();

	UFUNCTION(BlueprintCallable)
	void EndVrRightCounter();

	//

	// Components Section

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* sphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USceneComponent* sceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFloatingPawnMovement* movementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* swimAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* hurtAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAudioComponent* healAudioComponent;


	//


	//Variables Section

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBraking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forceScaler;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float oxygen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxOxygen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float stressLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float minimumStressLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maximumStressLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float timeBeforeStressRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPlayerIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forwardLeftCounter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forwardRightCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float oxygenPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float stressPercentage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsPhysicsActor;
};
