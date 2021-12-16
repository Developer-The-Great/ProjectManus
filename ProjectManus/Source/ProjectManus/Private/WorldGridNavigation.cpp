// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGridNavigation.h"
#include "AStarGridPathfindingQueryFilter.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

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
			FIntVector startNode = navigationGridActor->GetGridPositionFromWorldPosition(Query.StartLocation) + FIntVector(1, 0, 0);
			FIntVector endNode = navigationGridActor->GetGridPositionFromWorldPosition(Query.EndLocation) - FIntVector(-1,0,0);

			AStarGridPathfindingQueryFilter queryFilter;
			queryFilter.SetNavigationActor(navigationGridActor);

			TArray<FIntVector> gridPath;
			EGraphAStarResult aStarResult = pathfinder->FindPath(startNode, endNode, queryFilter, gridPath);

			if (aStarResult == EGraphAStarResult::InfiniteLoop || aStarResult == EGraphAStarResult::SearchFail)
			{
				pathfindingResult.Result = ENavigationQueryResult::Fail;
				return pathfindingResult;
			}

			gridPath.Add( gridPath[gridPath.Num() - 1] + FIntVector(1, 0, 0) );

			TArray<FVector> curvedWorldPath;

			if (gridPath.Num() <= 2)
			{
				for (auto& gridPos : gridPath)
				{
					NavPath->GetPathPoints().Add( FNavPathPoint(navigationGridActor->GetWorldPositionFromGridPosition(gridPos)) );
				}

			}
			else
			{
				NavPath->GetPathPoints().Add(FNavPathPoint(navigationGridActor->GetWorldPositionFromGridPosition(gridPath[0])));

				FVector end;

				for (size_t i = 0; i < gridPath.Num() - 2; i++)
				{
					FVector start = navigationGridActor->GetWorldPositionFromGridPosition(gridPath[i]);
					FVector control = navigationGridActor->GetWorldPositionFromGridPosition(gridPath[i + 1]);

					end = navigationGridActor->GetWorldPositionFromGridPosition(gridPath[i + 2]);

					FVector startDiff = control - start;
					FVector endDiff = end - control;

					FVector bezierArray[4]{ start + startDiff * 0.5f,control,control,control + endDiff * 0.5f };

					FOccluderVertexArray vectorArray;
					FVector::EvaluateBezier(bezierArray, 8, vectorArray);

		
					for (int bezierIndex = 0; bezierIndex < vectorArray.Num(); bezierIndex++)
					{
						NavPath->GetPathPoints().Add(FNavPathPoint(vectorArray[bezierIndex]));
					}
					
				}

				NavPath->GetPathPoints().Add(FNavPathPoint(end));

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
