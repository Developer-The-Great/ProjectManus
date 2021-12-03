// Fill out your copyright notice in the Description page of Project Settings.


#include "GridNavigationActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGridNavigationActor::AGridNavigationActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	navigationGrid.Init(navigationGridBlockDimensions.X, navigationGridBlockDimensions.Y, navigationGridBlockDimensions.Z);
}

void AGridNavigationActor::SetNodeBlockedState(const FIntVector& gridPosition, bool newNodeState)
{
	navigationGrid.SetIndexAt(gridPosition, newNodeState);
}

void AGridNavigationActor::OnConstruction(const FTransform& Transform)
{
	UE_LOG(LogTemp, Warning, TEXT("NAVIGATION CONSTRUCTION!"));

	auto debugDrawUpdatedGrid = [this]()
	{
		FVector gridStartPosition = GetActorLocation();

		FVector centerOffset =
			FVector(navigationGridBlockDimensions.X * blockDimensions.X, navigationGridBlockDimensions.Y * blockDimensions.Y,
				navigationGridBlockDimensions.Z * blockDimensions.Z) * 0.5f;

		DrawDebugBox(GetWorld(), gridStartPosition + centerOffset, centerOffset, FColor::Blue, false, 5.0f, 100);

		for (int z = 0; z < navigationGridBlockDimensions.Z; z++)
		{
			for (int y = 0; y < navigationGridBlockDimensions.Y; y++)
			{
				for (int x = 0; x < navigationGridBlockDimensions.X; x++)
				{
					if ( ! IsGridCellWalkable({ x,y,z }) )
					{
						/*DrawDebugBox(GetWorld(),
							GetWorldPositionFromGridPosition({ x,y,z }),
							blockDimensions * 0.5f, FColor::Green, false, 5.0f, 100);*/
					}
				}
			}
		}

	};

	InitializeGridUsingEnviroment();
	debugDrawUpdatedGrid();

}

// Called when the game starts or when spawned
void AGridNavigationActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!isGridInit)
	{
		UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY INIT"));
		InitializeGridUsingEnviroment();
		isGridInit = true;
	}
	
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
		, gridPosition.Z * blockDimensions.Z + gridStartPosition.Z) 
		+ (blockDimensions * 0.5f);
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

void AGridNavigationActor::InitializeGridUsingEnviroment()
{
	navigationGrid.Init(navigationGridBlockDimensions.X, navigationGridBlockDimensions.Y, navigationGridBlockDimensions.Z);

	TArray<TEnumAsByte<EObjectTypeQuery> > objectTypeQuery;
	objectTypeQuery.Add(TEnumAsByte<EObjectTypeQuery>(ObjectTypeQuery1));
	UE_LOG(LogTemp, Warning, TEXT("blockDimensions %s"), *(blockDimensions.ToString()));
	TArray<AActor*> actorsToIgnore;

	for (int z = 0; z < navigationGridBlockDimensions.Z; z++)
	{
		for (int y = 0; y < navigationGridBlockDimensions.Y; y++)
		{
			for (int x = 0; x < navigationGridBlockDimensions.X; x++)
			{
				FVector center = GetWorldPositionFromGridPosition({ x,y,z });

				TArray<AActor*> foundActors;
				UKismetSystemLibrary::BoxOverlapActors(GetWorld(),
					center,
					blockDimensions * 0.5f, objectTypeQuery, nullptr, actorsToIgnore, foundActors);

				if (foundActors.Num() > 0)
				{
					navigationGrid.SetIndexAt({ x,y,z }, false);
				}
			}
		}
	}
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

	return data[index];
}

void Grid3DArray::SetIndexAt(FIntVector vectorIndex, bool newIndexState) 
{
	int index = CalculateIndex(vectorIndex);

	data[index] = newIndexState;


}
