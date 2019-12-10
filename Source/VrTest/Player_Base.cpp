// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineMinimal.h"
#include "Engine/Engine.h"


// Sets default values
APlayer_Base::APlayer_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitializeComponents();
	InitializeVariables();
	
}


// Called when the game starts or when spawned
void APlayer_Base::BeginPlay()
{
	Super::BeginPlay();
	oxygen = maxOxygen;
	
	bPlayerIsDead = false;
	bTutorialImageIsActive = false;
	stressPercentage = stressLevel / maximumStressLevel;
	oxygenPercentage = oxygen / maxOxygen;

	sphereCollision->SetSimulatePhysics(bIsPhysicsActor);

	if (movementComponent == nullptr)
	{
		 movementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent());
	}

	movementComponent->Acceleration = acceleration;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->Deceleration = deacceleration;

	SetStressStage();

	bCanStroke = true;
	canStrokeTimer = timeBtwStrokes;

}

// Called every frame
void APlayer_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;

	if (bIsInMainMenu)
	{
		oxygen = maxOxygen;
	}

	if (bPlayerIsDead == false)
	{
		bPlayerIsDead = bLowerOxygen();
	}

	if (canStrokeTimer > 0)
	{
		canStrokeTimer -= DeltaTime;
		bCanStroke = false;
	}
	else
	{
		bCanStroke = true;
	}
	if (bIsInTutorial)
	{
		TutorialMovement();
	}
}

// Called to bind functionality to input
void APlayer_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayer_Base::ForwardStroke(float force)
{
	if (bCanStroke == false)
	{
		return;
	}
	if (bIsStuckInNet)
	{
		return;
	}
	if (bIsInTutorial && bTutorialImageIsActive == false)
	{
		return;
	}
	FRotator rotation = Controller->GetControlRotation();
	const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::X);
	AddMovementInput(direction,force);	
	canStrokeTimer = timeBtwStrokes;
	if (swimAudioComponent->Sound == NULL)
	{
		return;
	}

	if (bTutorialImageIsActive)
	{
		bTutorialImageIsActive = false;
		ExecuteTutorialImageSpawn();
	}
	swimAudioComponent->Play();
}

void APlayer_Base::RaiseMinimumStressLevel(float addedStress)
{
	minimumStressLevel += addedStress;
	if (stressLevel < minimumStressLevel)
	{
		stressLevel = minimumStressLevel;
	}
	SetStressStage();
	stressPercentage = stressLevel / maximumStressLevel;
}

void APlayer_Base::OxygenResupply(float stressHealed, float oxygenReplenished)
{
	oxygen += oxygenReplenished;
	oxygenPercentage = oxygen / maxOxygen;
	stressLevel -= stressHealed;
	SetStressStage();
	if (oxygen > maxOxygen)
	{
		oxygen = maxOxygen;
	}
	if (healAudioComponent->Sound == NULL )
	{
		return;
	}
	healAudioComponent->Play();

	stressPercentage = stressLevel / maximumStressLevel;
}

void APlayer_Base::ObstacleHit(float stressDamage, float oxygenDamage)
{
	stressLevel += stressDamage;
	SetStressStage();
	stressPercentage = stressLevel / maximumStressLevel;
	oxygen -= (oxygen / 100) * oxygenDamage;
	oxygenPercentage = oxygen / maxOxygen;
	timeBeforeStressRecovery = 5;
	if (hurtAudioComponent->Sound == NULL )
	{
		return;
	}
	hurtAudioComponent->Play();

	
}

void APlayer_Base::SetStressStage()
{
	if (stressLevel > 90 && stressLevel <= 100)
	{
		currentStressStage = 5;
	}
	else if (stressLevel > 75 && stressLevel <= 90)
	{
		currentStressStage = 4;
	}
	else if (stressLevel > 60 && stressLevel <= 75)
	{
		currentStressStage = 3;
	}
	else if (stressLevel > 35 && stressLevel <= 60)
	{
		currentStressStage = 2;
	}
	else if (stressLevel > 0 && stressLevel <= 35)
	{
		currentStressStage = 1;
	}
}

bool APlayer_Base::bLowerOxygen()
{
	switch (currentStressStage)
	{
	case 1:
		oxygen -= 2 * deltaTime;
		break;

	case 2:
		oxygen -= 5 * deltaTime;
		break;

	case 3:
		oxygen -= 10 * deltaTime;
		break;

	case 4:
		oxygen -= 15 * deltaTime;
		break;

	case 5:
		oxygen -= 20 * deltaTime;
		break;
	default:
		break;
	}

	oxygenPercentage = oxygen / maxOxygen;
	if (oxygen <= 0)
	{
		return true;
	}
	return false;
}

void APlayer_Base::StressRecovery()
{
	if (timeBeforeStressRecovery > 0.0f)
	{
		timeBeforeStressRecovery--;
	}
	else if (stressLevel > minimumStressLevel)
	{
		stressLevel--;
	}

	stressPercentage = stressLevel / maximumStressLevel;
	oxygenPercentage = oxygen / maxOxygen;
}

void APlayer_Base::Brake()
{
	if (movementComponent == nullptr)
	{
		movementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent());
	}
	bIsBraking = true;
	movementComponent->Deceleration = brakingDeacceleration;
}

void APlayer_Base::UnBrake()
{
	if (movementComponent == nullptr)
	{
		movementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent());
	}
	bIsBraking = false;
	movementComponent->Deceleration = deacceleration;
}

void APlayer_Base::InitializeComponents()
{
	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	sphereCollision->SetSimulatePhysics(true);
	sphereCollision->SetSphereRadius(50.0f);
	RootComponent = sphereCollision;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	sceneComponent->SetupAttachment(sphereCollision);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(sceneComponent);
	camera->SetRelativeLocation(FVector(10.0f,0.0f,0.0f));

	movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	movementComponent->Acceleration = acceleration;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->Deceleration = deacceleration;
	movementComponent->TurningBoost = 8;

	swimAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Swim Audio Component"));
	swimAudioComponent->SetupAttachment(camera);
	swimAudioComponent->SetRelativeLocation(FVector(100, 0, 0));

	hurtAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Hurt Audio Component"));
	hurtAudioComponent->SetupAttachment(camera);
	hurtAudioComponent->SetRelativeLocation(FVector(100, 0, 0));

	healAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Heal Audio Component"));
	healAudioComponent->SetupAttachment(camera);
	healAudioComponent->SetRelativeLocation(FVector(100, 0, 0));

	ambientSFXAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Ambient SFX Audio Component"));
	ambientSFXAudioComponent->SetupAttachment(camera);
	ambientSFXAudioComponent->SetRelativeLocation(FVector(100, 0, 0));

	backGroundMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Back Ground Music"));
	backGroundMusicAudioComponent->SetupAttachment(camera);
	backGroundMusicAudioComponent->SetRelativeLocation(FVector(100, 0, 0));
}

void APlayer_Base::InitializeVariables()
{
	stressLevel = 1;
	minimumStressLevel = 1;
	maximumStressLevel = 100;
	oxygen = 1000;
	maxOxygen = 1000;
	forceScaler = 3;
	timeBeforeStressRecovery = 5;
	maxSpeed = 1000.0f;
	acceleration = 25000.0f;
	deacceleration = 100.0f;
	brakingDeacceleration = 500.0f;

	movementComponent->Acceleration = acceleration;
	movementComponent->MaxSpeed = maxSpeed;
	movementComponent->Deceleration = deacceleration;
	movementComponent->TurningBoost = 8;
	timeBtwStrokes = 0.5f;

}

void APlayer_Base::StartPcLeftCounter()
{
	forwardLeftCounter = UGameplayStatics::GetTimeSeconds(this);
}

void APlayer_Base::StartPcRightCounter()
{
	forwardRightCounter = UGameplayStatics::GetTimeSeconds(this);
}

void APlayer_Base::EndPcLeftCounter()
{
	float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
	if (time >= 1.5f)
	{
		ForwardStroke(forceScaler * 1.5f);
	}
	else
	{
		ForwardStroke(forceScaler * time);
	}

}

void APlayer_Base::EndPcRightCounter()
{
	float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
	if (time >= 1.5f)
	{
		ForwardStroke(forceScaler * 1.5f);
	}
	else
	{
		ForwardStroke(forceScaler * time);
	}
}

void APlayer_Base::StartVrLeftCounter()
{
	forwardLeftCounter = UGameplayStatics::GetTimeSeconds(this);
	bCanLeftStroke = true;
}

void APlayer_Base::StartVrRightCounter()
{
	forwardRightCounter = UGameplayStatics::GetTimeSeconds(this);
	bCanRightStroke = true;
}

void APlayer_Base::EndVrLeftCounter()
{
	if (bCanLeftStroke)
	{
		float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
		bCanLeftStroke = false;
		if (time < 1.5f)
		{
			ForwardStroke(forceScaler * (1.5f - time));
		}
	}
}

void APlayer_Base::EndVrRightCounter()
{
	if (bCanRightStroke)
	{
		float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
		bCanRightStroke = false;
		if (time < 1.5f)
		{
			ForwardStroke(forceScaler * (1.5f - time));
		}
	}
}

void APlayer_Base::ExecuteTutorialImageSpawn_Implementation()
{
	// do something
}

UCameraComponent* APlayer_Base::CameraGetter()
{
	return camera;
}

void APlayer_Base::SetSFXVolume(float newVolume)
{
	if (newVolume > 1)
	{
		newVolume = 1;
	}
	else if (newVolume < 0)
	{
		newVolume = 0;
	}
	swimAudioComponent->VolumeModulationMin = newVolume;
	swimAudioComponent->VolumeModulationMax = newVolume;
	hurtAudioComponent->VolumeModulationMin = newVolume;
	hurtAudioComponent->VolumeModulationMax = newVolume;
	healAudioComponent->VolumeModulationMin = newVolume;
	healAudioComponent->VolumeModulationMax = newVolume;
	ambientSFXAudioComponent->VolumeModulationMin = newVolume;
	ambientSFXAudioComponent->VolumeModulationMax = newVolume;
}

void APlayer_Base::SetMusicVolume(float newVolume)
{
	if (newVolume > 1)
	{
		newVolume = 1;
	}
	else if (newVolume < 0)
	{
		newVolume = 0;
	}
	backGroundMusicAudioComponent->VolumeModulationMin = newVolume;
	backGroundMusicAudioComponent->VolumeModulationMax = newVolume;
}

void APlayer_Base::SetTutorialStatus(bool status)
{
	if (bSkipTutorial)
	{
		return;
	}
	bIsInTutorial = status;
	pathIndex = 0;
	timeSinceTutorialBegan = 0;
	tutorialStoppingTimer = tutorialPath[pathIndex].stoppingTime;
}

void APlayer_Base::TutorialMovement()
{
	timeSinceTutorialBegan += deltaTime;
	if (tutorialPath.Num() > 0)
	{
		if (tutorialPath[pathIndex].path == NULL)
		{
			if (pathIndex < tutorialPath.Num() - 1)
			{
				timeSinceTutorialBegan = (timeSinceTutorialBegan / 1.5f);
				pathIndex++;
				tutorialStoppingTimer = tutorialPath[pathIndex].stoppingTime;		
			}
			else
			{
				if (bTutorialImageIsActive == false)
				{
					bTutorialImageIsActive = true;
					SetTutorialStatus(false);
					ExecuteTutorialImageSpawn();
				}
			}
		}
		else
		{
			FVector newLocation = FVector(FMath::VInterpConstantTo(GetActorLocation(), tutorialPath[pathIndex].path->GetActorLocation(), timeSinceTutorialBegan, tutorialPath[pathIndex].interpSpeed));
			SetActorLocation(newLocation);
			if (GetActorLocation() == tutorialPath[pathIndex].path->GetActorLocation())
			{
				if (pathIndex < tutorialPath.Num() - 1)
				{
					if (tutorialPath[pathIndex].bStopsAtTargetPoint)
					{
						if (tutorialStoppingTimer <= 0)
						{
							timeSinceTutorialBegan = (timeSinceTutorialBegan / 1.5f);
							pathIndex++;
							tutorialStoppingTimer = tutorialPath[pathIndex].stoppingTime;

						}
						else
						{
							tutorialStoppingTimer -= deltaTime;

						}
					}
					else
					{
						timeSinceTutorialBegan = (timeSinceTutorialBegan / 1.5f);
						pathIndex++;
						tutorialStoppingTimer = tutorialPath[pathIndex].stoppingTime;
						/*	if (GEngine)
							{
								GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Incremented path"));
							}*/
					}
				}
				else
				{
					if (bTutorialImageIsActive == false)
					{
						bTutorialImageIsActive = true;
						SetTutorialStatus(false);
						ExecuteTutorialImageSpawn();
					}
				}
			}
		}
		
	}
}


