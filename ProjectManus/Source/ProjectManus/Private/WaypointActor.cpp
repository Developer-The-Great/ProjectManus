// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointActor.h"
#include "Components/SceneComponent.h"
#include "GridNavigationActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWaypointActor::AWaypointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	RootComponent = sceneComponent;
}

// Called when the game starts or when spawned
void AWaypointActor::BeginPlay()
{
	Super::BeginPlay();

	bool drawResult = false;

	//Init(true);
	
}

// Called every frame
void AWaypointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AWaypointActor::CalculateOffsetedWaypointDestination()
{
	//UE_LOG(LogTemp, Warning, TEXT("CalculateOffsetedWaypointDestination()"));

	if (possiblePositionIndices.Num() == 0) { return GetActorLocation(); }


	//UE_LOG(LogTemp, Warning, TEXT("possiblePositionIndices.Num() != 0"));
	//calculate random index
	
	FVector resultPosition;

	//find fill state
	bool foundFilled = false;
	
	for (bool fillState : possiblePositionFillState)
	{
		if (fillState)
		{
			foundFilled = true;
			break;
		}
	}

	int finalIndex = -1;

	if (foundFilled)
	{
		//UE_LOG(LogTemp, Warning, TEXT("FURTHEST POINT FIND"));

		TArray<float> positionScore;
		positionScore.Init(-1.0f, possiblePositionFillState.Num());
		
		for (size_t i = 0; i < possiblePositionIndices.Num(); i++)
		{
			bool bIsVacant = possiblePositionFillState[i];
			if (!bIsVacant)
			{
				FIntPoint point = possiblePositionIndices[i];

				positionScore[i] = 0.0f;

				for (size_t j = 0; j < possiblePositionFillState.Num(); j++)
				{
					if (possiblePositionFillState[j])
					{
						FIntPoint diff = possiblePositionIndices[j] - point;
						positionScore[i] += (FMath::Abs(diff.X) + FMath::Abs(diff.Y));
					}
				}


			}
		}

		float largestValueFound = TNumericLimits<float>::Lowest();
		int positionIndex = 0;

		for (size_t i = 0; i < positionScore.Num(); i++)
		{
			if (positionScore[i] > largestValueFound)
			{
				largestValueFound = positionScore[i];
				positionIndex = i;
			}
		}

		
		finalIndex = positionIndex;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("RANDOM POS FIND"));
		finalIndex = FMath::RandRange(0, possiblePositionIndices.Num() - 1);

	}
	
	possiblePositionFillState[finalIndex] = true;

	FIntPoint indexPosition = possiblePositionIndices[finalIndex];

	FVector lowestAvailablePosition = GetActorLocation()
		+ FVector(0.0f, -halfNumYBlocks * offset, 0.0f) + FVector(0.0f, 0.0f, -halfNumZBlocks * offset);

	resultPosition = lowestAvailablePosition
		+ FVector(0.0f, indexPosition.X * offset, 0.0f) + FVector(0.0f, 0.0f, indexPosition.Y * offset);

	//DrawDebugSphere(GetWorld(), resultPosition, 50.0f, 5, FColor::Red, true);

	return resultPosition;
}

void AWaypointActor::OnConstruction(const FTransform& Transform)
{
	bool drawResult = true;
	Init(drawResult);
}

void AWaypointActor::Init(bool drawResult)
{
	AActor* gridActor = UGameplayStatics::GetActorOfClass(GetWorld(), AGridNavigationActor::StaticClass());
	possiblePositionIndices.Empty();

	if (gridActor)
	{
		AGridNavigationActor* gridNavigationActor = dynamic_cast<AGridNavigationActor*>(gridActor);

		FVector lowestAvailablePosition = GetActorLocation()
			+ FVector(0.0f, -halfNumYBlocks * offset, 0.0f) + FVector(0.0f, 0.0f, -halfNumZBlocks * offset);

		UE_LOG(LogTemp, Warning, TEXT("GRID ACTOR %s"), *(gridNavigationActor->GetActorLocation().ToString()));
		

		for (int z = 0; z < (halfNumZBlocks * 2); z++)
		{
			for (int y = 0; y < (halfNumYBlocks * 2); y++)
			{
				FVector blockPosition = lowestAvailablePosition
					+ FVector(0.0f, y * offset, z * offset);

				FIntVector positionInGrid = gridNavigationActor->GetGridPositionFromWorldPosition(blockPosition);

				auto isValidAndWalkable = [&gridNavigationActor](FIntVector positionInGrid)->bool
				{
					return gridNavigationActor->IsGridCellValid(positionInGrid) && gridNavigationActor->IsGridCellWalkable(positionInGrid);
				};

				
				
				if (isValidAndWalkable(positionInGrid) 
					&& isValidAndWalkable(positionInGrid + FIntVector(-1,0,0) )
					&& isValidAndWalkable(positionInGrid + FIntVector( 1,0,0) )
					&& isValidAndWalkable(positionInGrid + FIntVector(-2,0,0) )
					&& isValidAndWalkable(positionInGrid + FIntVector(2, 0,0) ))
				{
					if (drawResult)
					{
						/*DrawDebugBox(GetWorld(),
							blockPosition,
							FVector(offset * 0.5f), FColor::Red, false, 5.0f, 100);*/
					}

					possiblePositionIndices.Add(FIntPoint(y, z));
					
				}
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GRID ACTOR AVAILABLE"));
	}

	UE_LOG(LogTemp, Warning, TEXT("possiblePositionIndices add %d"), possiblePositionIndices.Num() );
	possiblePositionFillState.Init(false, possiblePositionIndices.Num());
}

