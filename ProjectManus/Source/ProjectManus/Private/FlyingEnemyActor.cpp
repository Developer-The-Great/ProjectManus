// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingCharacterPawn.h"
#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"

#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "WaypointOrganizer.h"

// Sets default values
AFlyingEnemyActor::AFlyingEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	RootComponent = planeMesh;

	planeMesh->OnComponentHit.AddDynamic(this, &AFlyingEnemyActor::OnCollision);
	
	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("healthComponent"));
	healthComponent->SetupAttachment(planeMesh);

	movementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("movementComponent"));
	
	//mainBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("mainBoxComponent"));
	//mainBoxComponent->SetupAttachment(planeMesh);
	//mainBoxComponent->SetRelativeLocation(FVector(0, 0, 500), false, nullptr, ETeleportType::TeleportPhysics);
	//mainBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyActor::OnPathBlocked);
}	

void AFlyingEnemyActor::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT WALL"));
	if (OtherActor == player)
	{
		Destroy();
	}
	
}

void AFlyingEnemyActor::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PROJECTILE damage "));

	if ( dynamic_cast<AProjectile*>(OtherActor) )
	{
		
		float damageApplied = UGameplayStatics::ApplyDamage( this, 1.0f, nullptr, OtherActor, UDamageType::StaticClass() );

		UE_LOG( LogTemp, Warning, TEXT("ENEMY HIT PROJECTILE damage %f"), damageApplied );
		//UGameplayStatics::ApplyPointDamage(this,)
	}
}

void AFlyingEnemyActor::OnPathBlocked(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
	, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("PATH BLOCKED DIRECTION SHIFT REQUIRED"));
}

void AFlyingEnemyActor::HealthChangedCallback( float newHealth, float Damage, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("HealthChangedCallback %f"),newHealth);

	if (newHealth <= 0.0f)
	{
		AProjectile* projectile = dynamic_cast<AProjectile*>(DamageCauser);

		UE_LOG(LogTemp, Warning, TEXT("ENEMY DEATH"));

		if (projectile)
		{
			AActor* projectileSpawner = projectile->GetSpawnerActor();
			UE_LOG(LogTemp, Warning, TEXT("		...BY PROJECTILE"));
			if (projectileSpawner == player)
			{
				AFlyingCharacterPawn* playerAsPawn = dynamic_cast<AFlyingCharacterPawn*>(projectileSpawner);
				playerAsPawn->AddScore(pointsReceived * 0.5f);
				UE_LOG(LogTemp, Warning, TEXT("		...PLAYER SCORE ADDED"));
			}

		}

	}

}

FVector AFlyingEnemyActor::GetNextWaypoint() const
{
	if (waypointOrganizer)
	{
		return waypointOrganizer->GetNextWaypoint(currentWaypointIndex);
	}

	return FVector();
}

void AFlyingEnemyActor::UpdateWaypoint()
{
	if (waypointOrganizer)
	{
		waypointOrganizer->UpdateWaypoint(currentWaypointIndex);
	}
}

void AFlyingEnemyActor::UpdateMovementSpeedWithLayerSystem()
{

	float playerProgress = waypointOrganizer->CalculateLayerProgress(0);
	float selfProgress = waypointOrganizer->CalculateWaypointProgress( GetActorLocation() );

	float diff = playerProgress - selfProgress;

	int mult = diff < 0 ? -1 : 1;
	float boost = FMath::Abs(diff) > 0.1f ? 100.0f : 0;


	UE_LOG(LogTemp, Warning, TEXT("diff %f"), diff);
	UE_LOG(LogTemp, Warning, TEXT("boost %f"), boost);

	movementComponent->MaxSpeed += ( 50 * (diff )) + boost * mult;

	movementComponent->MaxSpeed = FMath::Clamp( movementComponent->MaxSpeed, 1000.0f, 5000.0f );

}

void AFlyingEnemyActor::SetWaypointOrganizer(AWaypointOrganizer* aWaypointOrganizer)
{
	waypointOrganizer = aWaypointOrganizer;
}

// Called when the game starts or when spawned
void AFlyingEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FVector(0, 0, 1).ToOrientationRotator());
	planeMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyActor::OnProjectileOverlap);
	healthComponent->OnHealthChangedEvent.AddDynamic(this, &AFlyingEnemyActor::HealthChangedCallback);

}

// Called every frame
void AFlyingEnemyActor::Tick(float DeltaTime)
{
	if (bHasReachedStartPoint)
	{
		//UpdateMovementSpeedWithLayerSystem();
	}

}

