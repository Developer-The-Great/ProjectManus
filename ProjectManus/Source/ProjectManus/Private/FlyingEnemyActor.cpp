// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingEnemyActor.h"
#include "Kismet/GameplayStatics.h"

#include "Projectile.h"

// Sets default values
AFlyingEnemyActor::AFlyingEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	planeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("planeMesh"));
	RootComponent = planeMesh;

	planeMesh->OnComponentHit.AddDynamic(this, &AFlyingEnemyActor::OnCollision);
	
	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("healthComponent"));
	healthComponent->SetupAttachment(planeMesh);
}

void AFlyingEnemyActor::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT WALL"));
	Destroy();
}

void AFlyingEnemyActor::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (dynamic_cast<AProjectile*>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT PROJECTILE"));

		float damageApplied = UGameplayStatics::ApplyDamage(this, 1.0f, nullptr, OtherActor, UDamageType::StaticClass());

		UE_LOG(LogTemp, Warning, TEXT("ENEMY HIT PROJECTILE damage %f"),damageApplied);
		//UGameplayStatics::ApplyPointDamage(this,)
	}
}

// Called when the game starts or when spawned
void AFlyingEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorRotation(FVector(0, 0, 1).ToOrientationRotator());
	planeMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemyActor::OnProjectileOverlap);
}

// Called every frame
void AFlyingEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorUpVector() * speed * DeltaTime,true,nullptr);
}

