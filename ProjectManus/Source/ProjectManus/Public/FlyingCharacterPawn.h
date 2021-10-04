// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlyingCharacterPawn.generated.h"

class UManusComponent;
class AProjectile;

UCLASS()
class PROJECTMANUS_API AFlyingCharacterPawn : public APawn
{
	GENERATED_BODY()

private:

	UPROPERTY(Category="Mesh", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class UStaticMeshComponent* planeMesh;

	UPROPERTY(Category="Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class UCameraComponent* camera;

	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true") )
	class USpringArmComponent* cameraSpring;

	UPROPERTY(Category = "Movement", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float characterSpeed = 30.0f;

	UManusComponent* handComponentToFollow = nullptr;

	
	void FireProjectile();

public:

	UFUNCTION(BlueprintCallable, Category = "Gesture Detection")
	void OnReceivePoseResults(TArray<float> poseValue);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AProjectile> projectileToSpawn;

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
