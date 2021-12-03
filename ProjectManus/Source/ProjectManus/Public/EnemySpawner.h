// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AFlyingEnemyActor;
class AWaypointActor;
class UBoxComponent;
class AWaypointOrganizer;

UCLASS()
class PROJECTMANUS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = "Spawn Parameters", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* onPlayerEnterTrigger = nullptr;

	UPROPERTY(Category = "Spawn Parameters", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* spawnLocation = nullptr;

	UPROPERTY(Category = "Spawn Parameters", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* goToAfterSpawnDestination = nullptr;

	UPROPERTY(Category = "Spawn Parameters", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFlyingEnemyActor> classToSpawn;

	UPROPERTY(Category = "Spawn Parameters", EditAnywhere, meta = (AllowPrivateAccess = "true"))
	AWaypointActor* firstWayPointDestination;

	AWaypointOrganizer* waypointOrganizer = nullptr;

	AActor* playerActor;

	bool bHasPlayerEntered = false;

	UFUNCTION()
	void OnOverlapComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
