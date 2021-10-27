// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMANUS_API UShieldComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(Category = "Shield", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldActiveTime = 3.0f;

	UPROPERTY(Category = "Shield", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldActivatorActiveTime = 8.0f;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool isShieldActive = false;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool canShieldBeActivated;

	FTimerHandle shieldActivatorTimeHandle;
	FTimerHandle shieldActiveTimeHandle;


public:

	UShieldComponent();

	bool TryActivateShield();

	bool GetShieldActiveState() const { return isShieldActive; }

	void DeactivateShield();

	void ReActivateShieldActivator() { canShieldBeActivated = true; }
};
