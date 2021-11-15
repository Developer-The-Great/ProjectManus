// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarGridPathfindingQueryFilter.h"
#include "GridNavigationActor.h"

AStarGridPathfindingQueryFilter::AStarGridPathfindingQueryFilter()
{
}

AStarGridPathfindingQueryFilter::~AStarGridPathfindingQueryFilter()
{
}

float AStarGridPathfindingQueryFilter::GetHeuristicScale() const
{
	return 1.0f;
}

float AStarGridPathfindingQueryFilter::GetHeuristicCost(const FIntVector& StartNode, const FIntVector& EndNode) const
{
	return FMath::Abs(StartNode.X - EndNode.X) + FMath::Abs(StartNode.Y - EndNode.Y) 
		+ FMath::Abs(StartNode.Z - EndNode.Z);
}

float AStarGridPathfindingQueryFilter::GetTraversalCost(const FIntVector& StartNode, const FIntVector& EndNode) const
{
	return 1.0f;
}

bool AStarGridPathfindingQueryFilter::IsTraversalAllowed(const FIntVector NodeA, const FIntVector NodeB) const
{
	constexpr int twoDimensionTraversalAmount = 2;
	constexpr int threeDimensionTraversalAmount = 3;

	if (!gridNavigationActor->IsGridCellWalkable(NodeA) || !gridNavigationActor->IsGridCellWalkable(NodeB))
	{
		return false;
	}

	//get traversal vector
	FIntVector traversalVector = NodeB - NodeA;

	//check how many dimensions this path is traversing
	TArray<int, TFixedAllocator<3>> traversalDelta{ traversalVector.X,traversalVector.Y, traversalVector.Z };

	int dimensionsTraversed = 0;

	for (int traversalAmount : traversalDelta)
	{
		bool isDimensionTraversed = traversalAmount != 0;

		if (isDimensionTraversed)
		{
			dimensionsTraversed++;
		}
	}

	auto* gridActor = gridNavigationActor;
	FIntVector startNode = NodeA;

	auto isNeigborOfDimensionWalkable = [gridActor,&traversalDelta, startNode](int dimensionIndex)->bool
	{
		TArray<int, TFixedAllocator<3>> tempArray{ 0,0,0 };

		tempArray[dimensionIndex] = traversalDelta[dimensionIndex];

		FIntVector neighborsOffset(tempArray[0], tempArray[1], tempArray[2]);

		return gridActor->IsGridCellWalkable(startNode + neighborsOffset);
	};

	if (dimensionsTraversed == twoDimensionTraversalAmount)
	{

		for (size_t i = 0; i < traversalDelta.Num(); i++)
		{
			bool isDimensionTraversed = traversalDelta[i] != 0;

			if ( isDimensionTraversed && !isNeigborOfDimensionWalkable(i) )
			{
				return false;
			}
		}


	}
	else if (dimensionsTraversed == threeDimensionTraversalAmount)
	{
		for (size_t i = 0; i < traversalDelta.Num(); i++)
		{
			if (!isNeigborOfDimensionWalkable(i))
			{
				return false;
			}
		}

		FIntVector diagonalNeigborInXYPlaneOffset  (traversalVector.X, traversalVector.Y, 0);
		FIntVector diagonalNeigborInXYPlaneOffset2 (traversalVector.X, 0, traversalVector.Z);
		FIntVector diagonalNeigborInXYPlaneOffset3 (0, traversalVector.Y, traversalVector.Z);

		if (!gridActor->IsGridCellWalkable(startNode + diagonalNeigborInXYPlaneOffset) ||
			!gridActor->IsGridCellWalkable(startNode + diagonalNeigborInXYPlaneOffset2)||
			!gridActor->IsGridCellWalkable(startNode + diagonalNeigborInXYPlaneOffset3) )
		{
			return false;
		}
	}

	return true;
}

bool AStarGridPathfindingQueryFilter::WantsPartialSolution() const
{
	return true;
}

void AStarGridPathfindingQueryFilter::SetNavigationActor(AGridNavigationActor* newGridNavigationActor)
{
	gridNavigationActor = newGridNavigationActor;
}
