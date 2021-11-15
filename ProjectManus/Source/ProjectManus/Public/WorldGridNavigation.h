// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationData.h"
#include "GraphAStar.h"
#include "AStarGridGraph.h"
#include "GridNavigationActor.h"
#include "WorldGridNavigation.generated.h"


class AGridNavigationActor;
/**
 * 
 */
UCLASS()
class PROJECTMANUS_API AWorldGridNavigation : public ANavigationData
{
	GENERATED_BODY()

public:

	AWorldGridNavigation();

	virtual void BeginPlay() override;

	static FPathFindingResult FindGridPath(const FNavAgentProperties& AgentProperties, const FPathFindingQuery& Query);

private:

	AStarGridGraph graph;
	FGraphAStar<AStarGridGraph>* Pathfinder = nullptr;

};
