// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "HighlightManager.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API AHighlightManager : public AActor
{
	GENERATED_BODY()
	
public:
	AHighlightManager(const FObjectInitializer & ObjectInitializer);

	UPROPERTY()
		TArray<AActor *> HighlightedActors;
	UPROPERTY()
		TArray<float> HighlightedCounters;

	void SERVER_AddActor(AActor * Actor, float Length);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void UpdateHighlights(float DeltaTime);
};
