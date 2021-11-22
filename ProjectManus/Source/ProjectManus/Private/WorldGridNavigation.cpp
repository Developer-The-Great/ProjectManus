// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGridNavigation.h"
#include "AStarGridPathfindingQueryFilter.h"
#include "EngineUtils.h"

AWorldGridNavigation::AWorldGridNavigation()
{
	FindPathImplementation = FindGridPath;
	FindHierarchicalPathImplementation = FindGridPath;

	Pathfinder = new FGraphAStar<AStarGridGraph>(graph);
}

void AWorldGridNavigation::BeginPlay()
{
	for (TActorIterator<AGridNavigationActor> It(GetWorld(), AGridNavigationActor::StaticClass()); It; ++It)
	{
		graph.SetNavigationActor(*It);
		break;
	}
}

FPathFindingResult AWorldGridNavigation::FindGridPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query)
{
	const ANavigationData* setNavigationData = Query.NavData.Get();
	const AWorldGridNavigation* gridNavigationData = (const AWorldGridNavigation*)( setNavigationData );

	//[1] Make sure path is valid

	FPathFindingResult pathfindingResult(ENavigationQueryResult::Error);
	pathfindingResult.Path = Query.PathInstanceToFill.IsValid() ?
		Query.PathInstanceToFill : 
		gridNavigationData->CreatePathInstance<FNavigationPath>(Query);

	FNavigationPath* NavPath = pathfindingResult.Path.Get();

	auto pathfinder = gridNavigationData->Pathfinder;
	auto navigationGridActor = gridNavigationData->graph.GetNavigationActor();


	if (NavPath)
	{
		if ((Query.StartLocation - Query.EndLocation).IsNearlyZero())
		{
			pathfindingResult.Path->GetPathPoints().Reset();
			pathfindingResult.Path->GetPathPoints().Add(FNavPathPoint(Query.EndLocation));
			pathfindingResult.Result = ENavigationQueryResult::Success;
		}
		else
		{
			FIntVector startNode = navigationGridActor->GetGridPositionFromWorldPosition(Query.StartLocation);
			FIntVector endNode = navigationGridActor->GetGridPositionFromWorldPosition(Query.EndLocation);

			AStarGridPathfindingQueryFilter queryFilter;
			queryFilter.SetNavigationActor(navigationGridActor);

			TArray<FIntVector> gridPath;
			EGraphAStarResult aStarResult = pathfinder->FindPath(startNode, endNode, queryFilter, gridPath);

			if (aStarResult == EGraphAStarResult::InfiniteLoop || aStarResult == EGraphAStarResult::SearchFail)
			{
				UE_LOG(LogTemp, Warning, TEXT("AWorldGridNavigationData A* fail: %d"), aStarResult);

				pathfindingResult.Result = ENavigationQueryResult::Fail;
				return pathfindingResult;
			}

			gridPath.Insert(startNode, 0);
			
			for (const FIntVector& gridPoint : gridPath)
			{
				NavPath->GetPathPoints().Add(FNavPathPoint(navigationGridActor->GetWorldPositionFromGridPosition(gridPoint)));
			}

			NavPath->MarkReady();
			pathfindingResult.Result = ENavigationQueryResult::Success;
		}
		
	}

	return pathfindingResult;
}

bool AWorldGridNavigation::ProjectPoint(const FVector& Point, FNavLocation& OutLocation, const FVector& Extent, FSharedConstNavQueryFilter Filter, const UObject* Querier) const
{
	FIntVector gridPosition = graph.GetNavigationActor()->GetGridPositionFromWorldPosition(Point);
	OutLocation = FNavLocation(Point);

	return graph.GetNavigationActor()->IsGridCellValid(gridPosition);
}
