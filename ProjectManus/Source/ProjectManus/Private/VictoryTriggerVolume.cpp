// Fill out your copyright notice in the Description page of Project Settings.

#include "VictoryTriggerVolume.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FlyingCharacterPawn.h"

AVictoryTriggerVolume::AVictoryTriggerVolume()
{

}

void AVictoryTriggerVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Victory!"));
		AFlyingCharacterPawn* character = dynamic_cast<AFlyingCharacterPawn*>(player);
		character->OnGameFinished.Broadcast(true);

	}

}

void AVictoryTriggerVolume::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AVictoryTriggerVolume::OnOverlapBegin);

	//DrawDebugBox(GetWorld(), GetActorLocation(), GetActorScale() * 100, FColor::Cyan, true, -1, 0, 5);

	player = UGameplayStatics::GetActorOfClass(GetWorld(), AFlyingCharacterPawn::StaticClass());
}
