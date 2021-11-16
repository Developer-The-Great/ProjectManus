// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridNavigationActor.generated.h"

class Grid3DArray
{
private:

	TArray<bool> data;
	FIntVector gridArrayDimensions;

	int CalculateIndex(FIntVector vectorIndex) const;

public:

	FIntVector GetArrayDimension() const { return gridArrayDimensions; }

	void Init(size_t width, size_t breadth, size_t height);

	bool GetIndexAt(FIntVector vectorIndex) const;

	void SetIndexAt(FIntVector vectorIndex, bool newIndexState);

	int32 Size() const { return data.Num(); }
};


UCLASS()
class PROJECTMANUS_API AGridNavigationActor : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(Category = "Navigation Parameters", EditDefaultsOnly)
	FIntVector navigationGridBlockDimensions;

	UPROPERTY(Category = "Navigation Parameters", EditDefaultsOnly)
	FVector blockDimensions;

	// Sets default values for this actor's properties
	AGridNavigationActor();

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void SetNodeBlockedState(const FIntVector& gridPosition, bool newNodeState);

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	FVector GetWorldPositionFromGridPosition(const FIntVector& gridPosition) const;

	FIntVector GetGridPositionFromWorldPosition(const FVector& gridPosition) const;

	bool IsGridCellWalkable(const FIntVector& gridPosition) const;

	bool IsGridCellValid(const FIntVector& gridPosition) const;

private:

	void InitializeGridUsingEnviroment();

	Grid3DArray navigationGrid;
	bool isGridInit = false;
};
