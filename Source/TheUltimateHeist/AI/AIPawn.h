// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIPawn.generated.h"

UINTERFACE(MinimalAPI)
class UAIPawn : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class THEULTIMATEHEIST_API IAIPawn
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		TArray<AActor *> GetActivelySensedActors();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		void SetActivelySensedActors(const TArray<AActor *> & Actors);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		TArray<AActor *> GetSensedActorsKeys();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		void SetSensedActorsKeys(const TArray<AActor *> & Keys);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		TArray<float> GetSensedActorsValues();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "AI Sensing")
		void SetSensedActorsValues(const TArray<float> & Values);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Sensing")
		float UpdateStimuli(AActor * Actor, float StimulusAmount, FVector ReceiverLocation, FVector StimulusLocation, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI Sensing")
		void DetectedActor(AActor * Actor);
};