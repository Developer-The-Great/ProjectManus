// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointOrganizer.generated.h"

class AFlyingEnemyActor;
class AWaypointActor;

UCLASS()
class PROJECTMANUS_API AWaypointOrganizer : public AActor
{

	GENERATED_BODY()

private:
	TArray<AWaypointActor*> waypointArray;

	TWeakObjectPtr<AActor> playerActor = nullptr;

	UPROPERTY(Category = "AI Layer System", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<float> layerOffset;

	void UpdateLayers();

	
public:	
	// Sets default values for this actor's properties
	AWaypointOrganizer();

	FVector GetNextWaypoint(int waypointIndex) const;

	void UpdateWaypoint(int& waypointIndex);

	int GetWaypointCount() const { return waypointArray.Num(); }

	float CalculateWaypointProgress(const FVector& layerPoint) const;

	float CalculateLayerProgress(int index);

	std::pair<FVector, FVector> GetWaypointLine(int index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
