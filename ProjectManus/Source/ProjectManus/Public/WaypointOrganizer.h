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
	
public:	
	// Sets default values for this actor's properties
	AWaypointOrganizer();
	
	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextWaypoint(AFlyingEnemyActor* enemyActor) const;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void UpdateWaypoint(AFlyingEnemyActor* enemyActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
