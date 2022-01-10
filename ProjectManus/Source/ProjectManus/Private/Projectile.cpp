// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DrawDebugHelpers.h"
#include "FlyingCharacterPawn.h"
#include "FlyingEnemyActor.h"
#include "Engine/StaticMeshActor.h"

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
	//UE_LOG(LogTemp, Warning, TEXT("OnProjectileOverlap FROM PROJECTILE %s"),*(OtherActor->GetName()) );

	if (OtherActor == spawnerActor)
	{
		return;
		
	}
	
	bool isOtherProjectile = dynamic_cast<AProjectile*>(OtherActor) != nullptr;
	bool isEnviroment = dynamic_cast<AStaticMeshActor*>(OtherActor) != nullptr; //|| dynamic_cast<AFlyingEnemyActor*>(OtherActor) == nullptr;
	

	if(isEnviroment) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("isEnviroment"));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("NOT Enviroment"));
	}


	if (isOtherProjectile || isEnviroment)
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
	SetActorLocation( newLocation ,true);


}

