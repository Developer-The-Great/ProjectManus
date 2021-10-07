// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingEnemyActor.h"

// Sets default values
AFlyingEnemyActor::AFlyingEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	RootComponent = planeMesh;

	planeMesh->OnComponentHit.AddDynamic(this, &AFlyingEnemyActor::OnProjectileCollision);

}

void AFlyingEnemyActor::OnProjectileCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

// Called when the game starts or when spawned
void AFlyingEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FVector(0, 0, 1).ToOrientationRotator());
	
}

// Called every frame
void AFlyingEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorUpVector() * speed * DeltaTime,true,nullptr);
}

