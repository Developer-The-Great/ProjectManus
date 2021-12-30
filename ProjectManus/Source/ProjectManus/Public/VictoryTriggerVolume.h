// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "VictoryTriggerVolume.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMANUS_API AVictoryTriggerVolume : public ATriggerVolume
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;

private:

	AActor* player = nullptr;

public:

	AVictoryTriggerVolume();

	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
};
