// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphAStar.h"

class AGridNavigationActor;

/**
 * 
 */
class PROJECTMANUS_API AStarGridPathfindingQueryFilter
{
public:
	AStarGridPathfindingQueryFilter();
	~AStarGridPathfindingQueryFilter();

	void SetNavigationActor(AGridNavigationActor* newGridNavigationActor);

	float GetHeuristicScale() const;														
	float GetHeuristicCost(const FIntVector & StartNode, const FIntVector& EndNode) const;
	float GetTraversalCost(const FIntVector& StartNode, const FIntVector& EndNode) const;
	bool IsTraversalAllowed(const FIntVector NodeA, const FIntVector NodeB) const;
	bool WantsPartialSolution() const;	

private:

	AGridNavigationActor* gridNavigationActor;

};
