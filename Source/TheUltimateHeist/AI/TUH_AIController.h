// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TUH_AIController.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATUH_AIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly)
		UAIPerceptionComponent * AIPerception;

	UFUNCTION(BlueprintCallable, Category = "AI|Interactions")
		TArray<AActor *> GetSensedActors(TArray<float> & AlertedValues);

protected:
	TMap<AActor *, float> SensedActors;
	TArray<AActor *> ActivelySensedActors;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnAIPerceptionUpdated(TArray<AActor*> Actors);
};
