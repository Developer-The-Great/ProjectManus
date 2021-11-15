// Fill out your copyright notice in the Description page of Project Settings.


#include "AStarGridGraph.h"
#include "GridNavigationActor.h"

AStarGridGraph::AStarGridGraph()
{
}

AStarGridGraph::~AStarGridGraph()
{
}

void AStarGridGraph::SetNavigationActor(AGridNavigationActor* newGridNavigationActor)
{
	gridNavigationActor = newGridNavigationActor;
}

AGridNavigationActor* AStarGridGraph::GetNavigationActor() const
{
	return gridNavigationActor;
}

bool AStarGridGraph::IsValidRef(FNodeRef NodeRef) const
{
	return gridNavigationActor->IsGridCellValid(NodeRef);
}

AStarGridGraph::FNodeRef AStarGridGraph::GetNeighbour(const FNodeRef& NodeRef, const int32 NeighbourIndex) const
{
	TArray<FIntVector, TFixedAllocator<26>> NeigborDelta
	{
		//XY PLANE
			FIntVector(1, 0, 0),
			FIntVector(0, 1, 0),
			FIntVector(1, 1, 0),

			FIntVector(-1, 0, 0),
			FIntVector(0, -1, 0),
			FIntVector(-1, -1, 0),

			FIntVector(-1, 1, 0),
			FIntVector(1, -1, 0),

		//XY Plane + Z

			FIntVector( 1, 0, 1),
			FIntVector( 0, 1, 1),
			FIntVector( 1, 1, 1),

			FIntVector(-1, 0, 1),
			FIntVector( 0,-1, 1),
			FIntVector(-1,-1, 1),

			FIntVector(-1, 1, 1),
			FIntVector( 1,-1, 1),

		//XY Plane - Z
			FIntVector( 1, 0,-1),
			FIntVector( 0, 1,-1),
			FIntVector( 1, 1,-1),

			FIntVector(-1, 0,-1),
			FIntVector( 0,-1,-1),
			FIntVector(-1,-1,-1),

			FIntVector(-1, 1,-1),
			FIntVector( 1,-1,-1),

		 //ABOVE AND BELOW
			FIntVector(0, 0, -1),
			FIntVector(0, 0,  1),
	};

	return NodeRef + NeigborDelta[NeighbourIndex];
}

int32 AStarGridGraph::GetNeighbourCount(FNodeRef NodeRef) const
{
	return 26;
}
