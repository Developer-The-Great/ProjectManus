// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldComponent.h"

void UShieldComponent::BeginPlay()
{
	Super::BeginPlay();
	shieldHealthCurrent = shieldHealthMax;
}

UShieldComponent::UShieldComponent()
{
	canShieldBeActivated = true;
	SetShieldActive(false);
}

bool UShieldComponent::TryActivateShield()
{
	if (canShieldBeActivated)
	{
		shieldHealthCurrent = shieldHealthMax;
		UE_LOG(LogTemp, Warning, TEXT("Activated Shield!"));

		SetShieldActive(true);
		canShieldBeActivated = false;
		OnShieldEnabled.Broadcast();

		return true;

	}

	return false;
}

void UShieldComponent::SetShieldActive( bool newShieldState )
{
	isShieldActive = newShieldState;
	SetVisibility(newShieldState);
}

void UShieldComponent::DamageShield(float damage)
{
	shieldHealthCurrent -= damage;

	if (shieldHealthCurrent <= 0.0f)
	{
		SetShieldActive(false);

		OnShieldDisabled.Broadcast();

		GetOwner()->GetWorldTimerManager().SetTimer
		(
			shieldActivatorTimeHandle,
			this,
			&UShieldComponent::ReActivateShieldActivator,
			shieldActivatorActiveTime);

	}

}
