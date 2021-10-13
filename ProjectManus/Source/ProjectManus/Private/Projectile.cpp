// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = planeMesh;

}

void AProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!spawnerActor) { return;  }

	if (OtherActor != spawnerActor)
	{
		Destroy(); 
	}
	
}

void AProjectile::BindOverlap()
{
	planeMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileOverlap);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLocation = GetActorLocation() + GetActorForwardVector() * projectileSpeed * DeltaTime;
	SetActorLocation( newLocation );

	/*DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * projectileSpeed,
		FColor(255, 0, 0),
		false, -1, 100,
		12.333
	);*/
}

