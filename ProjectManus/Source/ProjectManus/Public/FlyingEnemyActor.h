// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingEnemyActor.generated.h"

UCLASS()
class PROJECTMANUS_API AFlyingEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyingEnemyActor();

	UPROPERTY(Category = "Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* planeMesh = nullptr;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float speed = 30.0f;

	UFUNCTION()
	void OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
