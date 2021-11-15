// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreTrackerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTMANUS_API UScoreTrackerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float timeSinceInitializationSeconds;
	float currentScore;

public:	
	// Sets default values for this component's properties
	UScoreTrackerComponent();

	void AddScore(float scoreAddition);

	float GetCurrentScore() const { return currentScore; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


		
};
