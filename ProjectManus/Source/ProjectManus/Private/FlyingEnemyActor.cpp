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
#include "ShooterComponent.h"
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

	shooterComponent = CreateDefaultSubobject<UShooterComponent>(TEXT("ShooterComponent"));
	shooterComponent->SetupAttachment(planeMesh);

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
	AProjectile* projectile = dynamic_cast<AProjectile*>(OtherActor);

	if (!projectile) { return; }

	bool bIsNotOwnProjectileOrUninitialized = projectile->GetSpawnerActor() != this && projectile->GetSpawnerActor() != nullptr;

	if (bIsNotOwnProjectileOrUninitialized)
	{
		float damageApplied = UGameplayStatics::ApplyDamage( this, 1.0f, nullptr, OtherActor, UDamageType::StaticClass() );
		OnHitByProjectile();
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

void AFlyingEnemyActor::UpdateMovementSpeedWithLayerSystem(float DeltaTime)
{

	float playerProgress = waypointOrganizer->CalculateLayerProgress(0);

	float selfProgress = calculateSelfWaypointProgress();

	float diff = playerProgress - selfProgress;

	float mult = 1.0f;

	float threshold = 0.17f;

	if (diff > threshold)
	{
		mult = 0.75f + ( (diff - threshold) / threshold );
		movementComponent->MaxSpeed = FMath::FInterpTo(movementComponent->MaxSpeed, 3000, DeltaTime, 100 * mult);
	}
	else if (diff < -threshold)
	{
		mult = 0.5f + ( (FMath::Abs(diff) - threshold) / threshold);
		movementComponent->MaxSpeed = FMath::FInterpTo(movementComponent->MaxSpeed, 1000, DeltaTime, 100 * mult);
	}
	else
	{
		movementComponent->MaxSpeed = FMath::FInterpTo(movementComponent->MaxSpeed, 2000, DeltaTime, 100);
	}

	//UE_LOG(LogTemp, Warning, TEXT("player progress %f"), playerProgress);
	//UE_LOG(LogTemp, Warning, TEXT("enemy progress %f"), selfProgress);




}

void AFlyingEnemyActor::SetWaypointOrganizer(AWaypointOrganizer* aWaypointOrganizer)
{
	waypointOrganizer = aWaypointOrganizer;
}

void AFlyingEnemyActor::ShootPlayer()
{
	//no player cant shoot player
	if (!player.IsValid()) { UE_LOG(LogTemp, Warning, TEXT("ERROR NO PLAYER"));  return; }

	//[1] Calculate Direction needed to shoot player
	FVector forward = player->GetActorForwardVector();

	FVector playerLoc = player->GetActorLocation();
	FVector selfLoc = shooterComponent->GetComponentLocation();

	FVector selfToPlayer = selfLoc - playerLoc;

	AFlyingCharacterPawn* playerAsAFCP = dynamic_cast<AFlyingCharacterPawn*>(player.Get());
	
	float E = shooterComponent->GetProjectileSpeed();
	float P = playerAsAFCP->GetSpeed();

	float c = (P * P) - (E * E);

	float dx = FVector::DotProduct(selfToPlayer, forward);

	//enemy is behind player, dont shoot
	if (dx < 100.0f) { UE_LOG(LogTemp, Warning, TEXT("ENEMY BEHIND PLAYER"));  return; }

	float dy = ((selfToPlayer) - (forward * dx)).Size();

	float b = -2 * ( dx / dy) * P;

	float a =  ( (dx*dx) / (dy*dy) ) + 1;

	float b2Min4ac = (b * b) - (4 * a * c);
	
	float b2Min4acRoot = FMath::Sqrt(b2Min4ac);

	float firstResult = (-b + b2Min4acRoot) / (2 * a);
	float secondResult = (-b - b2Min4acRoot) / (2 * a);


	float vy = FMath::Max(firstResult, secondResult);
	float vx = FMath::Sqrt( (E * E) - (vy * vy) );
	
	FVector parallelProjectileVec = -forward * vx;

	

	FVector closestPoint = FMath::ClosestPointOnInfiniteLine(playerLoc, playerLoc + forward, selfLoc);

	FVector perpendicularToPlayerVecNorm = closestPoint - selfLoc;
	perpendicularToPlayerVecNorm.Normalize();

	FVector perpendicularPlayerProjectile = perpendicularToPlayerVecNorm * vy;

	FVector finalVec = parallelProjectileVec + perpendicularPlayerProjectile;
	finalVec.Normalize();

	shooterComponent->SetProjectileSpeed(E);
	shooterComponent->SetProjectileDirection( finalVec );
	shooterComponent->FireProjectile();

	OnShootProjectile();
}

// Called when the game starts or when spawned
void AFlyingEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	planeMesh->SetRelativeRotation(FVector(0, 0, 1).ToOrientationRotator());

	planeMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyActor::OnProjectileOverlap);
	healthComponent->OnHealthChangedEvent.AddDynamic(this, &AFlyingEnemyActor::HealthChangedCallback);

	
	GetWorldTimerManager().SetTimer(projectileFiringTimeHandle,
		this,
		&AFlyingEnemyActor::ShootPlayer,
		3.0f, true);

}

// Called every frame
void AFlyingEnemyActor::Tick(float DeltaTime)
{
	UpdateMovementSpeedWithLayerSystem(DeltaTime);

	if ((currentWaypointIndex + 1) == finalWaypointIndex)
	{

		UE_LOG(LogTemp, Warning, TEXT("Cleared at final"));

		GetWorldTimerManager().ClearTimer(projectileFiringTimeHandle);
	}
	
	if (player.IsValid() )
	{
		planeMesh->SetWorldRotation( FVector::CrossProduct(player->GetActorLocation() - GetActorLocation(), upDir) .ToOrientationRotator());
	}
	else
	{
		GetWorldTimerManager().ClearTimer(projectileFiringTimeHandle);
		return;
	}

	


	FVector currentVelocity = movementComponent->Velocity;

	//UE_LOG(LogTemp, Warning, TEXT("velocity %s"),(*currentVelocity.ToString()));

	if( FMath::IsNearlyEqual( currentVelocity.Size() , 0.0f ) ) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("velocity of enemy is zero"));
		upDir = FVector::UpVector;
		return;
	}

	FVector enemyFaceDirection = GetActorLocation() - player->GetActorLocation();
	
	FVector forwardRightCross = UKismetMathLibrary::Cross_VectorVector(enemyFaceDirection, FVector(0, 0, 1));

	float velDotRight = UKismetMathLibrary::Dot_VectorVector(forwardRightCross, currentVelocity);
	float velDotInverseRight = UKismetMathLibrary::Dot_VectorVector(-forwardRightCross, currentVelocity);

	float sign = velDotRight > velDotInverseRight ? 1 :-1;

	forwardRightCross *= sign;

	currentVelocity.Normalize();
	enemyFaceDirection.Normalize();
	forwardRightCross.Normalize();

	float velDotActorForwardNegated =  UKismetMathLibrary::Abs(UKismetMathLibrary::Dot_VectorVector(enemyFaceDirection, currentVelocity));

	FVector up = UKismetMathLibrary::Cross_VectorVector(enemyFaceDirection,FVector::RightVector);
	up.Normalize();

	if (UKismetMathLibrary::Dot_VectorVector(up, FVector::UpVector) < 0.0f)
	{
		up *= -1.0f;
	}

	FVector newUpDir = (forwardRightCross * (1.0f - velDotActorForwardNegated) ) + (up * (velDotActorForwardNegated) );
	newUpDir.Normalize();
	
	upDir = FMath::Lerp(upDir, newUpDir, 0.02f);
	upDir.Normalize();
	//upDir = newUpDir;
	UE_LOG(LogTemp, Warning, TEXT("forwardRightCross %s"), (*(forwardRightCross).ToString()));
	UE_LOG(LogTemp, Warning, TEXT("enemyFaceDirection %s"), (*(enemyFaceDirection).ToString()));
	UE_LOG(LogTemp, Warning, TEXT("velDotActorForwardNegated %f"), (velDotActorForwardNegated));
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + forwardRightCross * (1.0f - velDotActorForwardNegated) * 1000, FColor::Red, false, -1.0f, 0U, 30.0f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + up * (velDotActorForwardNegated) * 1000, FColor::Blue, false, -1.0f, 0U, 30.0f);
}

float AFlyingEnemyActor::calculateSelfWaypointProgress() const
{
	int currentWaypoint = currentWaypointIndex - 1;

	if (currentWaypoint < 0) { return 0.0f; }

	auto waypointLinePair = waypointOrganizer->GetWaypointLine(currentWaypoint);

	FVector closestPoint = UKismetMathLibrary::FindClosestPointOnSegment(GetActorLocation(), waypointLinePair.first, waypointLinePair.second);

	FVector waypointVec = waypointLinePair.second - waypointLinePair.first;

	float waypointLineLength;
	waypointVec.ToDirectionAndLength(waypointVec, waypointLineLength);
	
	return currentWaypoint + FVector::DotProduct(closestPoint - waypointLinePair.first,waypointVec)/waypointLineLength;
}

