// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, float, newHealth, float, Damage, AActor*, DamageCauser);

UCLASS()
class PROJECTMANUS_API UHealthComponent : public USceneComponent
{
	GENERATED_BODY()

private:



public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChangedSignature OnHealthChangedEvent;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float defaultHealth;

	UPROPERTY(VisibleAnywhere, Category = "Health")
		float currentHealth;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UFUNCTION()
	void OnHealthChanged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
