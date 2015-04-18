// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheUltimateHeistCharacter.h"
#include "TUH_AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_AICharacter : public ATheUltimateHeistCharacter
{
	GENERATED_BODY()
	
public:
	TArray<AActor *> ActivelySensedActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		TArray<AActor *> SensedActorsKeys;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
		TArray<float> SensedActorsValues;

	UFUNCTION(BlueprintImplementableEvent)
		float UpdateStimuli(AActor * Actor, float StimulusAmount, FVector ReceiverLocation, FVector StimulusLocation, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintImplementableEvent)
		void DetectedActor(AActor * Actor);
};
