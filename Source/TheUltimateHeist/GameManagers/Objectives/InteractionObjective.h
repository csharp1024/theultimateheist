// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Objective.h"
#include "InteractionObjective.generated.h"

class AInteractable;

UCLASS()
class THEULTIMATEHEIST_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		AInteractable * Interactable;
	
	virtual void StartObjective() override;
};
