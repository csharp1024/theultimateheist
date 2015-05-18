// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Objective.h"
#include "ObjectToLocationObjective.generated.h"

class ATriggerBase;

UCLASS()
class THEULTIMATEHEIST_API UObjectToLocationObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		AActor * Object;
	UPROPERTY(EditDefaultsOnly)
		ATriggerBase * Trigger;

	UFUNCTION()
		void OnOverlap(AActor * OtherActor);

	virtual void StartObjective() override;
};