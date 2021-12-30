// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterComponent.h"
#include "Projectile.h"
#include "DrawDebugHelpers.h"

void UShooterComponent::BeginFire()
{
	if (currentAmno > 0)
	{
		GetOwner()->GetWorldTimerManager().SetTimer(projectileFiringTimeHandle,
			this,
			&UShooterComponent::FireProjectile,
			timeBetweenShotSeconds, true);
	}
}

void UShooterComponent::FireProjectile()
{
	if( currentAmno <= 0)
	{
		EndFire();
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	AActor* actor = GetOwner();
	FVector actorForward = actor->GetActorForwardVector();

	if (useOwnerDir)
	{
		shootDirection = actor->GetActorForwardVector();
	}

	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(projectileToSpawn,
		GetComponentLocation() , shootDirection.Rotation(),
		spawnParams);

	if (projectile)
	{
		projectile->SetSpawnerActor(actor);
		projectile->SetProjectileSpeed(projectileSpeed);
		projectile->BindOverlap();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PROJECTILE SPAWNED"));
	}

	currentAmno--;

	OnShootProjectile.Broadcast();
}

void UShooterComponent::EndFire()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(projectileFiringTimeHandle);
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
	timeBetweenShotSeconds = 60.0f / FiringRPM;
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

