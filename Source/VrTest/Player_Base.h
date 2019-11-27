// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/TargetPoint.h"
#include "Player_Base.generated.h"


UCLASS(Blueprintable)
class VRTEST_API APlayer_Base : public APawn
{
	GENERATED_BODY()

private:

	//private functions

	bool bLowerOxygen();

	void InitializeComponents();

	void InitializeVariables();

	void SetStressStage();

	void TutorialMovement();

	//

	//private variables
	float deltaTime;

	bool bCanLeftStroke;

	bool bCanRightStroke;

	bool bCanStroke;

	float canStrokeTimer;

	int pathIndex;

	float timeSinceTutorialBegan;

	

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

	UFUNCTION( BlueprintCallable)
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

	UFUNCTION(BlueprintCallable)
	UCameraComponent* CameraGetter();

	UFUNCTION(BlueprintCallable)
	void SetAudioComponentVolumes(float newVolume);

	UFUNCTION(BlueprintCallable)
	void SetTutorialStatus(bool status);
	
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float forceScaler;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float oxygen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen System")
	float maxOxygen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float stressLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float minimumStressLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float maximumStressLevel = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen System")
	float timeBeforeStressRecovery;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Vars")
	bool bPlayerIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Vars")
	float forwardLeftCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Read Only Vars")
	float forwardRightCounter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float oxygenPercentage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oxygen System")
	float stressPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPhysicsActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Read Only Vars")
	bool bIsStuckInNet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float maxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float deacceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Speed")
	float brakingDeacceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
	float timeBtwStrokes;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Oxygen System", meta = (clampMin = "1", clampMax = "5"))
	int currentStressStage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInMainMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tutorial")
	bool bIsInTutorial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	TArray<ATargetPoint*> path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	float interpSpeed;
};
