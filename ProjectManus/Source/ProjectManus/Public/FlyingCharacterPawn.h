// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingCharacterPawn.generated.h"

class UManusComponent;
class AProjectile;
class UHealthComponent;
class UShooterComponent;

struct TickMovement
{
	float movementApplicationTimeSeconds =0.0f;
	FVector moveAmount = FVector();

	TickMovement() = default;

	TickMovement(FVector pVec, float pMovementTimeSeconds) 
		: movementApplicationTimeSeconds(pMovementTimeSeconds),moveAmount(pVec)
	{

	}
};

UCLASS()
class PROJECTMANUS_API AFlyingCharacterPawn : public APawn
{
	GENERATED_BODY()

private:

	UManusComponent* handComponentToFollow = nullptr;

	TQueue<TickMovement> movementApplicationQueue;

	FVector externalVelocity;

	int currentTick = 0;

	UPROPERTY(Category="Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class UStaticMeshComponent* planeMesh;

	UPROPERTY(Category="Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class UCameraComponent* camera;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class USpringArmComponent* cameraSpring;

	UPROPERTY(Category = "Weapon", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UShooterComponent* shooterComponent;

	UPROPERTY(Category = "Health", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* healthComponent;

	float timeBetweenShotSeconds = 1.0f;

	/** The Speed of the Player */
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float characterSpeed = 30.0f;

	float characterSpeedCached;

	/** The amount of time where the player is invulnerable to collisions*/
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float invulnerabilityTimeSeconds = 0.5f;

	/** How much the player bounces when it hits a wall */
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float onCollisionBounceFactor = 1.0f;

	void FireProjectile();

	void BeginFire();

	void EndFire();

	void applyBounce(FVector direction);

	void reApplyCharacterSpeed();

	void activateMovement();

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FTimerHandle collisionTimerHandle;
	bool bIsAbleToReactToCollision = true;

	FTimerHandle projectileFiringTimeHandle;
	

	int poseIndex = 0;

	bool bIsMovementEnabled = false;

public:

	UFUNCTION(BlueprintCallable, Category = "Gesture Detection")
	void OnReceivePoseResults(TArray<float> poseValue);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AProjectile> projectileToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FiringRPM = 600.0f;

	

	// Sets default values for this pawn's properties
	AFlyingCharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
