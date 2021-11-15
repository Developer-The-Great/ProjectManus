// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "FlyingEnemyActor.generated.h"

class AFlyingCharacterPawn;
class UBoxComponent;

UCLASS()
class PROJECTMANUS_API AFlyingEnemyActor : public AActor
{
	GENERATED_BODY()

private:
	AActor* player = nullptr;

	TArray<UBoxComponent*> movementOptionBoxes;

	FVector defaultLine;
	float mLineLength;
	
public:	
	// Sets default values for this actor's properties
	AFlyingEnemyActor();

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* planeMesh = nullptr;

	UPROPERTY(Category = "Health", VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* healthComponent = nullptr;

	UPROPERTY(Category = "CollisionAvoidance", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* mainBoxComponent;

	

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetPointsReceived() const { return pointsReceived; }

};
