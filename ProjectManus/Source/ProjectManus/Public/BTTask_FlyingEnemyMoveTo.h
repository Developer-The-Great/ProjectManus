// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_FlyingEnemyMoveTo.generated.h"

/**
 * 
 */
UCLASS(config = Game)
class PROJECTMANUS_API UBTTask_FlyingEnemyMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

	virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override;

	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
	
};
