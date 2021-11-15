// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GraphAStar.h"

class AGridNavigationActor;

/**
 * 
 */
class PROJECTMANUS_API AStarGridGraph
{
public:
	typedef FIntVector FNodeRef;

	AStarGridGraph();
	~AStarGridGraph();

	void SetNavigationActor(AGridNavigationActor* newGridNavigationActor);
	AGridNavigationActor* GetNavigationActor() const;

	bool IsValidRef(FNodeRef NodeRef) const;												
	FNodeRef GetNeighbour(const FNodeRef& NodeRef, const int32 NeighbourIndex) const;

	int32 GetNeighbourCount(FNodeRef NodeRef) const;

private:

	AGridNavigationActor* gridNavigationActor;
};
