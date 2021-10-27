// Fill out your copyright notice in the Description page of Project Settings.


#include "ShieldComponent.h"



UShieldComponent::UShieldComponent()
{
	canShieldBeActivated = true;
	DeactivateShield();
}

bool UShieldComponent::TryActivateShield()
{
	if (canShieldBeActivated)
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated Shield!"));

		isShieldActive = true;
		canShieldBeActivated = false;

		SetVisibility(true);

		GetOwner()->GetWorldTimerManager().SetTimer
		(
			shieldActiveTimeHandle,
			this,
			&UShieldComponent::DeactivateShield, 
			shieldActiveTime);

		GetOwner()->GetWorldTimerManager().SetTimer
		(
			shieldActivatorTimeHandle,
			this,
			&UShieldComponent::ReActivateShieldActivator,
			shieldActivatorActiveTime);

		return true;

	}

	return false;
}

void UShieldComponent::DeactivateShield()
{
	isShieldActive = false;
	SetVisibility(false);
}
