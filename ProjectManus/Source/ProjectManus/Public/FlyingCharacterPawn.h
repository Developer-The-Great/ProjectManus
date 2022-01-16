// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingCharacterPawn.generated.h"

class UManusComponent;
class UHealthComponent;
class UShooterComponent;
class UShieldComponent;
class UScoreTrackerComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFinishedSignature,bool, isPlayerWin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStartSignature);

UCLASS()
class PROJECTMANUS_API AFlyingCharacterPawn : public APawn
{
	GENERATED_BODY()

private:

	UManusComponent* handComponentToFollow = nullptr;
	UManusComponent* handForPoses = nullptr;

	FVector externalVelocity;

	int currentTick = 0;

	//------------------------------------- Default Components --------------------------------------------------// 

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

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UShieldComponent* shieldComponent;

	UPROPERTY(Category = "Scoring System", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UScoreTrackerComponent* scoreTrackerComponent;

	//------------------------------------- Movement Variables --------------------------------------------------// 

	/** The Speed of the Player */
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float characterSpeed = 30.0f;

	/** The amount of time where the player is invulnerable to collisions*/
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float invulnerabilityTimeSeconds = 0.5f;

	/** How much the player bounces when it hits a wall */
	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float onCollisionBounceFactor = 1.0f;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsUsingManusGloves = true;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float maximumAltitude = 3480.0f;

	UPROPERTY(Category = "Movement", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float maxTimeSpentInMaximumAltitudeSeconds = 3.0f;

	float currentTimeSpentInMaximumAltitudeSeconds = 0.0f;

	float characterSpeedCached;

	bool bIsMovementEnabled = false;

	//------------------------------------- Collisions --------------------------------------------------// 

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bIsAbleToReactToCollision = true;

	FTimerHandle collisionTimerHandle;

	//------------------------------------- Weapon Logic --------------------------------------------------// 

	void FireProjectile();
	void BeginFire();
	void EndFire();

	void UpdateReloadState(float deltaTime);

	//------------------------------------- Movement Logic --------------------------------------------------// 

	void reApplyCharacterSpeed();
	void activateMovement();
	void rotatePlayerBasedOnHand();
	void moveOnPlayerForward( float DeltaTime );
	void UpdateMaximumAltitudeTimeSpent( float DeltaTime );

	int poseIndex = 0;

	//------------------------------------- Shield Activation Logic --------------------------------------------------// 

	bool checkRightHandPointingUp();

	UPROPERTY(Category = "Shield Activation", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldActivationAngleDegrees = 30.0f;

	void DisablePlayer();

	UFUNCTION()
	void CharacterHealthChangedCallback(float newHealth, float Damage, AActor* DamageCauser);

	UFUNCTION()
		void FinishedGameCallback(bool isPlayerWin);

public:

	UFUNCTION(BlueprintCallable, Category = "Gesture Detection")
	void OnReceivePoseResults(TArray<float> poseValue);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int GetCurrentAmno() const;

	UFUNCTION(BlueprintCallable, Category = "ScoreTracking")
	float GetCurrentScore() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float CalculateRemainingTimeAtMaximumAltitude() const
	{ 
		return maxTimeSpentInMaximumAltitudeSeconds - currentTimeSpentInMaximumAltitudeSeconds; 
	};

	float GetSpeed() const { return characterSpeed; }

	void AddScore(float addedPoints);

	// Sets default values for this pawn's properties
	AFlyingCharacterPawn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void OnHitEnviroment();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void OnHitByProjectile();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void OnHitByLaser();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void OnFirstTimeAtAltitudeLimit();

	UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
		void OnHitShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameFinishedSignature OnGameFinished;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnGameStartSignature OnGameStart;

};
