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

FVector AWaypointOrganizer::GetNextWaypoint(int waypointIndex) const
{
	if (waypointArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WAYPOINTS IN ORGANIZER!"));
		return FVector();

	}

	return waypointArray[waypointIndex]->GetActorLocation();
}

void AWaypointOrganizer::UpdateWaypoint(int& waypointIndex)
{
	if (waypointArray.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO WAYPOINTS IN ORGANIZER!"));
		waypointIndex = 0;
		return;
	}

	waypointIndex++;
	waypointIndex = FMath::Clamp(waypointIndex, 0, waypointArray.Num() - 1);

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
		return LHS.GetWaypointIndex() < RHS.GetWaypointIndex();
	});

	UE_LOG(LogTemp, Warning, TEXT("number of waypoints found %d"), waypointArray.Num());
}

// Called every frame
void AWaypointOrganizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

