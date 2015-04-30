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

	UFUNCTION(BlueprintCallable, Category = "Killed")
		void Killed();

protected:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "AI Events")
		void OnAIPerceptionUpdated(TArray<AActor*> Actors);
};
