// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterComponent.h"

void UShooterComponent::BeginFire()
{
}

void UShooterComponent::FireProjectile()
{
	currentAmno--;
}

void UShooterComponent::EndFire()
{
}

// Sets default values for this component's properties
UShooterComponent::UShooterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShooterComponent::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}


// Called every frame
void UShooterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (currentReloadTime > reloadTime)
	{
		Reload();
	}
}

