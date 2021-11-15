// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNavigationActor.h"

// Sets default values
AGridNavigationActor::AGridNavigationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AGridNavigationActor::SetNodeBlockedState(const FIntVector& gridPosition, bool newNodeState)
{
	navigationGrid.SetIndexAt(gridPosition, newNodeState);
}

// Called when the game starts or when spawned
void AGridNavigationActor::BeginPlay()
{
	Super::BeginPlay();
	navigationGrid.Init(navigationGridBlockDimensions.X, navigationGridBlockDimensions.Y, navigationGridBlockDimensions.Z);
	
	
}

// Called every frame
void AGridNavigationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AGridNavigationActor::GetWorldPositionFromGridPosition(const FIntVector& gridPosition) const
{
	FVector gridStartPosition = GetActorLocation();

	return FVector(
		gridPosition.X * blockDimensions.X + gridStartPosition.X
		, gridPosition.Y * blockDimensions.Y + gridStartPosition.Y
		, gridPosition.Z * blockDimensions.Z + gridStartPosition.Z) + blockDimensions * 0.5f;
}

FIntVector AGridNavigationActor::GetGridPositionFromWorldPosition(const FVector& gridPosition) const
{
	FVector gridStartPosition = GetActorLocation();

	int x = (gridPosition.X - gridStartPosition.X) / blockDimensions.X;
	int y = (gridPosition.Y - gridStartPosition.Y) / blockDimensions.Y;
	int z = (gridPosition.Z - gridStartPosition.Z) / blockDimensions.Z;

	return FIntVector(x,y,z);
}

bool AGridNavigationActor::IsGridCellWalkable(const FIntVector& gridPosition) const
{
	return navigationGrid.GetIndexAt(gridPosition);
}

bool AGridNavigationActor::IsGridCellValid(const FIntVector& gridPosition) const
{

	FIntVector blockCount = navigationGrid.GetArrayDimension();

	return 
		gridPosition.X >= 0 && gridPosition.X < blockCount.X &&
		gridPosition.Y >= 0 && gridPosition.Y < blockCount.Y &&
		gridPosition.Z >= 0 && gridPosition.Z < blockCount.Z;
}

int Grid3DArray::CalculateIndex(FIntVector vectorIndex) const
{
	return vectorIndex.X + (gridArrayDimensions.X * vectorIndex.Y) + (gridArrayDimensions.X * gridArrayDimensions.Y * vectorIndex.Z);
}

void Grid3DArray::Init(size_t width, size_t breadth, size_t height)
{
	gridArrayDimensions = FIntVector(width, breadth, height);

	data.Init(true, width * breadth * height);
}

bool Grid3DArray::GetIndexAt(FIntVector vectorIndex) const
{
	int index = CalculateIndex(vectorIndex);

	if (vectorIndex == FIntVector(1, 1, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("BlockState %d"), data[index]);

	}
	return data[index];
}

void Grid3DArray::SetIndexAt(FIntVector vectorIndex, bool newIndexState) 
{
	int index = CalculateIndex(vectorIndex);

	data[index] = newIndexState;


}
