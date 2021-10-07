// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingCharacterPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "../../../Plugins/Manus/Source/Manus/Public/ManusComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Projectile.h"



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
}

// Called when the game starts or when spawned
void AFlyingCharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	//planeMesh->OnComponentHit.AddDynamic()
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
		}
	}

	planeMesh->OnComponentHit.AddDynamic(this, &AFlyingCharacterPawn::OnCollision);

	characterSpeedCached = characterSpeed;

	FTimerHandle moveDelay;

	GetWorldTimerManager().SetTimer(
		moveDelay, this, &AFlyingCharacterPawn::activateMovement, 3.0f, false);

}

// Called every frame
void AFlyingCharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//[1] Set Rotation of Pawn
	if (handComponentToFollow)
	{
		FVector hand = handComponentToFollow->GetSocketLocation("hand_r");
		FVector index = handComponentToFollow->GetSocketLocation("index_01_r");
		FVector middle = handComponentToFollow->GetSocketLocation("middle_01_r");

		FVector forward = middle - hand;
		forward.Normalize();

		FVector right = index - hand;

		FVector up = UKismetMathLibrary::Cross_VectorVector(right,forward); //get up from temp right
		up.Normalize();

		//find real right
		right = UKismetMathLibrary::Cross_VectorVector(forward,up);
		right.Normalize();

		FRotator newRot = UKismetMathLibrary::MakeRotationFromAxes(forward, right, up);
		newRot.Roll = -newRot.Roll;
		
		FQuat setRotation = FQuat::Slerp(GetActorRotation().Quaternion(), newRot.Quaternion(), 0.2f);


		SetActorRotation(setRotation, ETeleportType::TeleportPhysics);
		//UE_LOG(LogTemp, Warning, TEXT("frotator %s"), *handComponentToFollow->GetComponentRotation().ToString() );
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("handComponentToFollow is null"));
	}

	//[1] Move player
	if (bIsMovementEnabled)
	{
		externalVelocity += GetActorForwardVector() * characterSpeed * DeltaTime;
		externalVelocity *= 0.9f;

		FVector unitDirection; float length;
		externalVelocity.ToDirectionAndLength(unitDirection, length);

		//if (length > 50)
		//{
		//	externalVelocity = unitDirection * 50.0f;
		//	UE_LOG(LogTemp, Warning, TEXT("Clamped Speed ") );
		//}

		UE_LOG(LogTemp, Warning, TEXT("externalVelocity %s"), *(externalVelocity.ToString()));

		SetActorLocation(
			GetActorLocation() + externalVelocity,
			true,
			nullptr,
			ETeleportType::None);
	}
}

// Called to bind functionality to input
void AFlyingCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AFlyingCharacterPawn::FireProjectile()
{
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileToSpawn, GetActorLocation() + GetActorForwardVector() * 50.0f, GetActorRotation());

}

void AFlyingCharacterPawn::applyBounce(FVector direction)
{

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
	}


	// Deflect along the surface when we collide.
	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), Hit.Normal.ToOrientationQuat(), 0.01f));

}

void AFlyingCharacterPawn::OnReceivePoseResults(TArray<float> poseValue)
{
	//[1] Identify the pose that the right hand with the largest value in the array

	float largestPoseValue = -1.0;
	int foundIndex = 0;

	for (size_t i = 0; i < poseValue.Num(); i++)
	{
		if (poseValue[i] > largestPoseValue)
		{
			largestPoseValue = poseValue[i];
			foundIndex = i;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("found index is %d"), foundIndex);

	switch (foundIndex)
	{
	case 1:
		FireProjectile();
		break;
	default:
		break;
	}


}

