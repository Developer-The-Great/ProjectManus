// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "WaypointActor.h"
#include "FlyingEnemyActor.h"
#include "FlyingCharacterPawn.h"


void AEnemySpawner::OnOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bHasPlayerEntered && dynamic_cast<AFlyingCharacterPawn*>(OtherActor) )
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap Spawn"));
		bHasPlayerEntered = true;

		FActorSpawnParameters spawnParams;
		spawnParams.bNoFail = true;

		AActor* enemyAsActor = GetWorld()->SpawnActor(classToSpawn, &spawnLocation->GetComponentTransform(), spawnParams);
		AFlyingEnemyActor* enemy = dynamic_cast<AFlyingEnemyActor*>(enemyAsActor);

		enemy->DirectSetWaypointIndex(firstWayPointDestination->GetWaypointIndex());
		enemy->SetStartPoint(goToAfterSpawnDestination->GetComponentLocation());
	}
}

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	onPlayerEnterTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnTrigger"));
	spawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnLocationOnTrigger"));
	goToAfterSpawnDestination = CreateDefaultSubobject<USceneComponent>(TEXT("FirstDestinationAfterSpawn"));

	RootComponent = spawnLocation;

	onPlayerEnterTrigger->SetupAttachment(spawnLocation);
	goToAfterSpawnDestination->SetupAttachment(spawnLocation);

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	onPlayerEnterTrigger->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::OnOverlapComponent);

	if (!firstWayPointDestination)
	{
		UE_LOG(LogTemp, Warning, TEXT("WAYPOINT NULL"));
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

