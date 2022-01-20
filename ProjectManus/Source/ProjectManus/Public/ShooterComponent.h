// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ShooterComponent.generated.h"

class AProjectile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileShootSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBeginReloadSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndReloadSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMANUS_API UShooterComponent : public USceneComponent
{
	GENERATED_BODY()

private:

	//------------------------------------- WEAPON VARIABLES --------------------------------------------------// 

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool useOwnerDir = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int defaultAmno;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int currentAmno;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float reloadTime = 3.0f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float currentReloadTime = 0.0f;

	FTimerHandle projectileFiringTimeHandle;
	float timeBetweenShotSeconds = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectile> projectileToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float FiringRPM = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	float projectileSpeed = 1000.0f;

	FVector shootDirection;
	FVector projectileSpawn;

public:	
	// Sets default values for this component's properties
	UShooterComponent();

	//------------------------------------- WEAPON LOGIC --------------------------------------------------// 

	int GetCurrentAmno() const { return currentAmno; }

	void BeginFire();
	void FireProjectile();
	void EndFire();

	void ClearReloadTime();
	void AddReloadTime(float dt);
	void Reload() 
	{
		currentAmno = defaultAmno; 
		ClearReloadTime();
	}
	
	void SetProjectileSpeed(float aProjectileSpeed) { projectileSpeed = aProjectileSpeed; }

	float GetProjectileSpeed() const { return projectileSpeed; }

	void SetProjectileDirection(FVector aProjectileDir) 
	{ 
		shootDirection = aProjectileDir; 
		shootDirection.Normalize();
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnProjectileShootSignature OnShootProjectile;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FBeginReloadSignature OnBeginReload;

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FEndReloadSignature OnEndReload;
		
};
