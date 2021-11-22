// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "FlyingEnemyActor.generated.h"


class AFlyingCharacterPawn;
class UBoxComponent;
class UFloatingPawnMovement;
class AWaypointOrganizer;

UCLASS()
class PROJECTMANUS_API AFlyingEnemyActor : public APawn
{
	GENERATED_BODY()

private:
	AActor* player = nullptr;

	TArray<UBoxComponent*> movementOptionBoxes;

	FVector defaultLine;
	float mLineLength;

	int currentWaypointIndex = 0;

	AWaypointOrganizer* waypointOrganizer = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AFlyingEnemyActor();

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* planeMesh = nullptr;

	UPROPERTY(Category = "Health", VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* healthComponent = nullptr;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* movementComponent = nullptr;

	int GetCurrentWaypointIndex() const { return currentWaypointIndex; }

	void SetWaypointIndex(int newIndex) { currentWaypointIndex = newIndex; }

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float speed = 30.0f;

	UPROPERTY(Category = "Scoring System", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float pointsReceived = 1000.0f;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPathBlocked( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor
		, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HealthChangedCallback(float newHealth, float Damage, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetNextWaypoint() const;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void UpdateWaypoint();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetPointsReceived() const { return pointsReceived; }

};
