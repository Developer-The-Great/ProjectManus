// Fill out your copyright notice in the Description page of Project Settings.
#include "WaypointOrganizer.h"
#include "DrawDebugHelpers.h"
#include "WaypointActor.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingEnemyActor.h"

void AWaypointOrganizer::UpdateLayers()
{


}

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
	UE_LOG(LogTemp, Warning, TEXT("GetNextWaypoint"));
	return waypointArray[waypointIndex]->CalculateOffsetedWaypointDestination();
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

	playerActor = GetWorld()->GetFirstPlayerController()->GetPawn();

	if (layerOffset.Num() == 0)
	{
		layerOffset.Add(500.0f);
		UE_LOG(LogTemp, Warning, TEXT("NO LAYER FOUND, DEFAULT LAYER HAS BEEN ADDED!"));
	}
}

// Called every frame
void AWaypointOrganizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float progress = CalculateLayerProgress(0);
	float index = FMath::TruncToFloat(progress);

	/*FVector start = waypointArray[static_cast<int>(index)]->GetActorLocation();
	FVector end = waypointArray[static_cast<int>(index) + 1]->GetActorLocation();*/

	//DrawDebugLine(
	//		GetWorld(),
	//		playerActor->GetActorLocation(),
	//		start + (end - start) * (progress - index),
	//	 	FColor::Red, false);

}

float AWaypointOrganizer::CalculateWaypointProgress(const FVector& layerPoint) const
{
	int closestWayPoint = -1;
	float lowestPointDistance = TNumericLimits<float>::Max();
	FVector closestPointInLine;

	for (size_t i = 0; i < waypointArray.Num() - 1; i++)
	{
		FVector currentPointInLine = FMath::ClosestPointOnSegment(layerPoint,
			waypointArray[i]->GetActorLocation(), waypointArray[i + 1]->GetActorLocation());

		float pointDistance = FVector(currentPointInLine - layerPoint).Size();

		if (pointDistance < lowestPointDistance)
		{
			lowestPointDistance = pointDistance;
			closestPointInLine = currentPointInLine;
			closestWayPoint = i;
		}
	}

	float pointInLineToStartDist = FVector(
		waypointArray[closestWayPoint]->GetActorLocation() -
		closestPointInLine).Size();

	float closestLineLength =
		FVector(
			waypointArray[closestWayPoint]->GetActorLocation() -
			waypointArray[closestWayPoint + 1]->GetActorLocation()).Size();

	return static_cast<float>(closestWayPoint) + ( pointInLineToStartDist / closestLineLength );
}

float AWaypointOrganizer::CalculateLayerProgress(int index)
{
	if (!playerActor) { return 1.0f; }

	FVector playerLoc = playerActor->GetActorLocation();

	int waypointIndex = static_cast<int>( CalculateWaypointProgress( playerLoc ) );

	if (waypointIndex == waypointArray.Num() - 1)
	{
		return waypointIndex;
	}

	FVector normWaypointDir(waypointArray[waypointIndex + 1]->GetActorLocation() -
		waypointArray[waypointIndex]->GetActorLocation());

	normWaypointDir.Normalize();

	FVector offset = normWaypointDir * layerOffset[0];

	FVector layerWorldLoc = playerLoc + offset;

	return CalculateWaypointProgress(layerWorldLoc);
}


