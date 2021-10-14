// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class PROJECTMANUS_API AProjectile : public AActor
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* planeMesh = nullptr;

	AActor* spawnerActor = nullptr;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float projectileSpeed = 400.0f;

	void SetSpawnerActor(AActor* newSpawnerActor) { spawnerActor = newSpawnerActor; };
	void BindOverlap();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
