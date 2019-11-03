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

	stressPercentage = stressLevel / maximumStressLevel;
	oxygenPercentage = oxygen / maxOxygen;

	sphereCollision->SetSimulatePhysics(bIsPhysicsActor);


}

// Called every frame
void APlayer_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;

	if (bPlayerIsDead == false)
	{
		bPlayerIsDead = LowerOxygen();
	}
}

// Called to bind functionality to input
void APlayer_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayer_Base::ForwardStroke(float force)
{
	FRotator rotation = Controller->GetControlRotation();
	const FVector direction = FRotationMatrix(rotation).GetScaledAxis(EAxis::X);
	AddMovementInput(direction, force);
	if (swimAudioComponent->Sound == NULL)
	{
		return;
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
	stressPercentage = stressLevel / maximumStressLevel;
}

void APlayer_Base::OxygenResupply(float stressHealed, float oxygenReplenished)
{
	oxygen += oxygenReplenished;
	oxygenPercentage = oxygen / maxOxygen;
	if (oxygen > maxOxygen)
	{
		oxygen = maxOxygen;
	}
	if (healAudioComponent->Sound == NULL)
	{
		return;
	}
	healAudioComponent->Play();

	stressPercentage = stressLevel / maximumStressLevel;
}

void APlayer_Base::ObstacleHit(float stressDamage, float oxygenDamage)
{
	stressLevel += stressDamage;
	stressPercentage = stressLevel / maximumStressLevel;
	oxygen -= (oxygen / 100) * oxygenDamage;
	oxygenPercentage = oxygen / maxOxygen;
	timeBeforeStressRecovery = 5;
	if (hurtAudioComponent->Sound == NULL)
	{
		return;
	}
	hurtAudioComponent->Play();
}

bool APlayer_Base::LowerOxygen()
{
	oxygen -= stressLevel * deltaTime;
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
	movementComponent->Deceleration = 500.0f;
}

void APlayer_Base::UnBrake()
{
	if (movementComponent == nullptr)
	{
		movementComponent = Cast<UFloatingPawnMovement>(GetMovementComponent());
	}
	bIsBraking = false;
	movementComponent->Deceleration = 100.0f;
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
	camera->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));

	movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	movementComponent->Acceleration = 25000;
	movementComponent->MaxSpeed = 1200;
	movementComponent->Deceleration = 1;
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
	canLeftStroke = true;
}

void APlayer_Base::StartVrRightCounter()
{
	forwardRightCounter = UGameplayStatics::GetTimeSeconds(this);
	canRightStroke = true;
}

void APlayer_Base::EndVrLeftCounter()
{
	if (canLeftStroke)
	{
		float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
		canLeftStroke = false;
		if (time < 1.5f)
		{
			ForwardStroke(forceScaler * (1.5f - time));
		}
	}


}

void APlayer_Base::EndVrRightCounter()
{
	if (canRightStroke)
	{
		float time = UGameplayStatics::GetTimeSeconds(this) - forwardLeftCounter;
		canRightStroke = false;
		if (time < 1.5f)
		{
			ForwardStroke(forceScaler * (1.5f - time));
		}
	}
}
