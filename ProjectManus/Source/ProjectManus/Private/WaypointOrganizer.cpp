// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointOrganizer.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingEnemyActor.h"

// Sets default values
AWaypointOrganizer::AWaypointOrganizer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector AWaypointOrganizer::GetNextWaypoint(AFlyingEnemyActor* enemyActor) const
{
	return waypointArray[enemyActor->GetCurrentWaypointIndex()]->GetActorLocation();
}

void AWaypointOrganizer::UpdateWaypoint(AFlyingEnemyActor* enemyActor)
{
	int currentIndex = enemyActor->GetCurrentWaypointIndex();
	currentIndex = FMath::Clamp(currentIndex, 0, waypointArray.Num() - 1);

	enemyActor->SetWaypointIndex(currentIndex);
}

// Called when the game starts or when spawned
void AWaypointOrganizer::BeginPlay()
{
	Super::BeginPlay();
	TSubclassOf<AWaypointActor> waypoint;

	TArray<AActor*> waypointActorBeforeCasted;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointActor::StaticClass(), waypointActorBeforeCasted);

	for (auto actor : waypointActorBeforeCasted)
	{
		auto waypointActor = dynamic_cast<AWaypointActor*>(actor);

		if (waypointActor->GetWaypointIndex() >= 0)
		{
			waypointArray.Add(dynamic_cast<AWaypointActor*>(actor));
		}
	}

	waypointArray.Sort([](const AWaypointActor& LHS, const AWaypointActor& RHS) 
	{ 
		return LHS.GetWaypointIndex() > RHS.GetWaypointIndex();
	});
}

// Called every frame
void AWaypointOrganizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

