// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointActor.generated.h"

class USceneComponent;

UCLASS()
class PROJECTMANUS_API AWaypointActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypointActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetWaypointIndex() const { return index; }

private:

	USceneComponent* sceneComponent = nullptr;

	FVector destinationOffset;
	FVector spawnOffset;

	UPROPERTY(Category = "Waypoint Index", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int index = -1;

};
