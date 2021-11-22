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

// Called when the game starts or when spawned
void AFlyingEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FVector(0, 0, 1).ToOrientationRotator());
	planeMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyActor::OnProjectileOverlap);
	healthComponent->OnHealthChangedEvent.AddDynamic(this, &AFlyingEnemyActor::HealthChangedCallback);

	player = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (!player)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PLAYER FOR ENEMY "));
	}

	AActor* foundWaypoint = UGameplayStatics::GetActorOfClass(GetWorld(), AWaypointOrganizer::StaticClass());

	if(!foundWaypoint) 
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WAYPOINT FOUND "));
		return;
	}

	waypointOrganizer = dynamic_cast<AWaypointOrganizer*>(foundWaypoint);

	

}

// Called every frame
void AFlyingEnemyActor::Tick(float DeltaTime)
{

	SetActorLocation(GetActorLocation() + GetActorUpVector() * speed * DeltaTime, true, nullptr);

	//FHitResult result;
	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);

	//FVector dir; float length;
	//defaultLine.ToDirectionAndLength(dir, length);

	//FVector transformLineVec = GetActorTransform().TransformVector( GetActorForwardVector() * mLineLength);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + transformLineVec, FColor(255, 0, 0), false);

	//if (GetWorld()->LineTraceSingleByChannel(result,
	//	GetActorLocation(), GetActorLocation() + transformLineVec, ECollisionChannel::ECC_Visibility, params) )
	//{

	//	mainBoxComponent->SetWorldLocation(GetActorLocation() + GetActorUpVector() * result.Distance, false, nullptr, ETeleportType::TeleportPhysics);
	//	
	//	UE_LOG(LogTemp, Warning, TEXT("VISIBILITY BOXES SHORTENED dir: %s %f"), *(dir.ToString() ) ,result.Distance);
	//}
	//else
	//{
	//	mainBoxComponent->SetWorldLocation( GetActorLocation() + transformLineVec,false,nullptr,ETeleportType::TeleportPhysics);
	//}

	//Super::Tick(DeltaTime);
	//DrawDebugPoint(GetWorld(), mainBoxComponent->GetComponentTransform().GetLocation()  , 5, FColor(52, 220, 239), false);
	//DrawDebugBox(GetWorld(), mainBoxComponent->GetComponentTransform().GetLocation(), mainBoxComponent->GetScaledBoxExtent(), FColor(52, 220, 239), false);
	//

	////if mainBoxComponent has any overlapping actors
	//TArray<AActor*> overlappingActors;
	//mainBoxComponent->GetOverlappingActors(overlappingActors);

	//for (UBoxComponent* boxComponent : movementOptionBoxes)
	//{
	//	TArray<AActor*> boxOverlappingActors;
	//	boxComponent->GetOverlappingActors(boxOverlappingActors);

	//	if (boxOverlappingActors.Num() == 0)
	//	{
	//		DrawDebugBox(GetWorld(), boxComponent->GetComponentTransform().GetLocation(), boxComponent->GetScaledBoxExtent(), FColor(0, 255, 0), false);
	//		
	//		UE_LOG(LogTemp, Warning, TEXT("Found debug box"));
	//	}
	//	else
	//	{
	//		DrawDebugBox(GetWorld(), boxComponent->GetComponentTransform().GetLocation(), boxComponent->GetScaledBoxExtent(), FColor(255, 0, 0), false);
	//		UE_LOG(LogTemp, Warning, TEXT("this box is overlapping"));
	//	}

	//}

	//if (overlappingActors.Num() == 0) { return; }

	//UE_LOG(LogTemp,Warning,TEXT("FOUND OVERLAPPING ACTORS DIVERT COURSE NOW"));

	//

	//

	//for (UBoxComponent* boxComponent : movementOptionBoxes)
	//{
	//	TArray<AActor*> boxOverlappingActors;
	//	boxComponent->GetOverlappingActors(boxOverlappingActors);

	//	if (boxOverlappingActors.Num() == 0)
	//	{
	//		FRotator CurrentRotation = GetActorRotation();

	//		FVector forward = (boxComponent->GetComponentTransform().GetLocation() - GetActorLocation());
	//		forward.Normalize();

	//		FVector right = UKismetMathLibrary::Cross_VectorVector(forward, GetActorUpVector());
	//		FVector up = UKismetMathLibrary::Cross_VectorVector(right, forward);

	//		FRotator newRot = UKismetMathLibrary::MakeRotationFromAxes(forward, right, up);

	//		SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), newRot.Quaternion(), 0.01f));
	//		break;
	//	}
	//}


}

