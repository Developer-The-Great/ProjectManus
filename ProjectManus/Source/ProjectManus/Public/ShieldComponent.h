// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ShieldComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldEnabledSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShieldDisabledSignature);

/**
 * 
 */
UCLASS()
class PROJECTMANUS_API UShieldComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
		
private:

	UPROPERTY(Category = "Shield", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldActivatorActiveTime = 8.0f;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool isShieldActive = false;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool canShieldBeActivated;

	UPROPERTY(Category = "Shield", VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldHealthCurrent;

	UPROPERTY(Category = "Shield", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float shieldHealthMax = 4.0f;

	FTimerHandle shieldActivatorTimeHandle;


	void SetShieldActive( bool newShieldState );

	void ReActivateShieldActivator() { canShieldBeActivated = true; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UShieldComponent();

	bool TryActivateShield();

	bool GetShieldActiveState() const { return isShieldActive; }

	void DamageShield(float damage);

	UFUNCTION(BlueprintCallable, Category = "Shield")
	float GetShieldHealth() const { return isShieldActive ? shieldHealthCurrent : 0.0f; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FShieldEnabledSignature OnShieldEnabled;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FShieldDisabledSignature OnShieldDisabled;
};
