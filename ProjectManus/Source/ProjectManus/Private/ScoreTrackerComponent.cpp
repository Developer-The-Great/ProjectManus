// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreTrackerComponent.h"

// Sets default values for this component's properties
UScoreTrackerComponent::UScoreTrackerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UScoreTrackerComponent::AddScore(float scoreAddition)
{
	currentScore += scoreAddition;
}


// Called when the game starts
void UScoreTrackerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UScoreTrackerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	timeSinceInitializationSeconds += DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("Current Score %f"), currentScore);
	// ...
}

