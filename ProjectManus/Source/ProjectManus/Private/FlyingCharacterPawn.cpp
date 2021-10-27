// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingCharacterPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ShooterComponent.h"
#include "HealthComponent.h"
#include "ShieldComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../../../Plugins/Manus/Source/Manus/Public/ManusComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "DrawDebugHelpers.h"

// Sets default values
AFlyingCharacterPawn::AFlyingCharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	RootComponent = planeMesh;

	cameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("cameraSpring"));
	cameraSpring->SetupAttachment(RootComponent);
	cameraSpring->bEnableCameraLag = false;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(cameraSpring);
	camera->bUsePawnControlRotation = false;

	shooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent"));
	shooterComponent->SetupAttachment(RootComponent);

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->SetupAttachment(RootComponent);

	shieldComponent = CreateDefaultSubobject<UShieldComponent>(TEXT("ShieldComponent"));
	shieldComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlyingCharacterPawn::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> foundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), foundActors);

	for (AActor* actor : foundActors)
	{
		TArray<UManusComponent*> manusSkeletons;
		actor->GetComponents(manusSkeletons);

		for (UManusComponent* manusComp : manusSkeletons)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Manus Component"));

			if (manusComp->MotionCaptureType == EManusMotionCaptureType::LeftHand)
			{
				UE_LOG(LogTemp, Warning, TEXT("	Left Hand Found"));
				handComponentToFollow = manusComp;
			}
			else if (manusComp->MotionCaptureType == EManusMotionCaptureType::RightHand)
			{
				UE_LOG(LogTemp, Warning, TEXT("	Right Hand Found"));
				handForPoses = manusComp;
			}
			
		}
	}

	planeMesh->OnComponentHit.AddDynamic(this, &AFlyingCharacterPawn::OnCollision);

	characterSpeedCached = characterSpeed;

	FTimerHandle moveDelay;

	GetWorldTimerManager().SetTimer(
		moveDelay, this, &AFlyingCharacterPawn::activateMovement, 13.0f, false);

}

// Called every frame
void AFlyingCharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (handComponentToFollow)
	{
		rotatePlayerBasedOnHand();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("handComponentToFollow is null"));
	}

	if (bIsMovementEnabled)
	{
		moveOnPlayerForward(DeltaTime);
	}

	UpdateReloadState(DeltaTime);

}

// Called to bind functionality to input
void AFlyingCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AFlyingCharacterPawn::FireProjectile()
{
	shooterComponent->FireProjectile();
}

void AFlyingCharacterPawn::BeginFire()
{
	shooterComponent->BeginFire();
}

void AFlyingCharacterPawn::EndFire()
{
	shooterComponent->EndFire();
}

void AFlyingCharacterPawn::UpdateReloadState(float deltaTime)
{
	if (poseIndex == 2)
	{
		shooterComponent->AddReloadTime(deltaTime);
	}
	else
	{
		shooterComponent->ClearReloadTime();
	}
}

void AFlyingCharacterPawn::reApplyCharacterSpeed()
{
	characterSpeed = characterSpeedCached;
	bIsAbleToReactToCollision = true;
}

void AFlyingCharacterPawn::activateMovement()
{
	bIsMovementEnabled = true;
}

void AFlyingCharacterPawn::rotatePlayerBasedOnHand()
{
	FVector hand = handComponentToFollow->GetSocketLocation("hand_r");
	FVector index = handComponentToFollow->GetSocketLocation("index_01_r");
	FVector middle = handComponentToFollow->GetSocketLocation("middle_01_r");
	
	FVector forward = middle - hand;
	forward.Normalize();

	FVector right = index - hand;

	FVector up = UKismetMathLibrary::Cross_VectorVector(right, forward); //get up from temp right
	up.Normalize();

	//find real right
	right = UKismetMathLibrary::Cross_VectorVector(forward, up);
	right.Normalize();

	FRotator newRot = UKismetMathLibrary::MakeRotationFromAxes(forward, right, up);
	newRot.Roll = -newRot.Roll;

	FQuat setRotation = FQuat::Slerp( GetActorRotation().Quaternion(), newRot.Quaternion(), 0.2f );

	SetActorRotation(setRotation, ETeleportType::TeleportPhysics);
}

void AFlyingCharacterPawn::moveOnPlayerForward( float DeltaTime )
{
	externalVelocity += GetActorForwardVector() * characterSpeed * DeltaTime;
	externalVelocity *= 0.9f;

	FVector unitDirection; float length;
	externalVelocity.ToDirectionAndLength(unitDirection, length);

	SetActorLocation(
		GetActorLocation() + externalVelocity,
		true,
		nullptr,
		ETeleportType::None);
}

bool AFlyingCharacterPawn::checkRightHandPointingUp()
{
	FVector hand = handForPoses->GetSocketLocation("hand_r");
	FVector middle = handForPoses->GetSocketLocation("middle_01_r");

	FVector forward = middle - hand;
	forward.Normalize();

	float dotResult = UKismetMathLibrary::Dot_VectorVector( forward, FVector::UpVector );
	float angleFromUp = UKismetMathLibrary::RadiansToDegrees( UKismetMathLibrary::Acos(dotResult) );

	UE_LOG(LogTemp, Warning, TEXT("angle %f"), angleFromUp);

	return angleFromUp < shieldActivationAngleDegrees;
}

void AFlyingCharacterPawn::OnCollision( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bIsAbleToReactToCollision)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *(Hit.Normal.ToString()));
		bIsAbleToReactToCollision = false;

		externalVelocity = UKismetMathLibrary::GetReflectionVector( externalVelocity , Hit.Normal) + Hit.Normal * onCollisionBounceFactor;

		//characterSpeed = 0.0f;

		GetWorldTimerManager().SetTimer(
			collisionTimerHandle, this, &AFlyingCharacterPawn::reApplyCharacterSpeed, invulnerabilityTimeSeconds, false);

		if ( ! shieldComponent->GetShieldActiveState())
		{
			UGameplayStatics::ApplyDamage(this, 1.0f, nullptr, OtherActor, UDamageType::StaticClass());
		}
	
	}


	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), Hit.Normal.ToOrientationQuat(), 0.01f));

}

void AFlyingCharacterPawn::OnReceivePoseResults(TArray<float> poseValue)
{
	//[1] Identify the pose that the right hand with the largest value in the array

	float largestPoseValue = -1.0;
	int currentFoundIndex = 0;

	for (size_t i = 0; i < poseValue.Num(); i++)
	{
		if (poseValue[i] > largestPoseValue)
		{
			largestPoseValue = poseValue[i];
			currentFoundIndex = i;
		}
	}

	if (poseIndex != 1 && currentFoundIndex == 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER -> BEGIN FIRE"));
		BeginFire();
	}

	if (poseIndex == 1 && currentFoundIndex != 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("PLAYER -> END FIRE"));
		EndFire();
	}

	//if pose index equals shield and fingers are pointing up and shield activator is up 

	bool isRightHandPointingUp = false;

	if ( poseIndex == 3 && checkRightHandPointingUp() )
	{
		bool activated = shieldComponent->TryActivateShield();
		if (activated) { UE_LOG(LogTemp, Warning, TEXT("PLAYER -> SHIELD %d"), activated) };
		
	}

		//apply shield
		//set timer for shield off
		//set timer for shield activator

	poseIndex = currentFoundIndex;

}

int AFlyingCharacterPawn::GetCurrentAmno() const
{
	return shooterComponent->GetCurrentAmno();
}

