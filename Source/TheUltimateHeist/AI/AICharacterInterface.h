// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AICharacterInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAICharacterInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IAICharacterInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
		float UpdateStimuli(AActor * Actor, FVector ReceiverLocation, FVector StimulusLocation, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintImplementableEvent)
		void DetectedActor(AActor * Actor);
};
