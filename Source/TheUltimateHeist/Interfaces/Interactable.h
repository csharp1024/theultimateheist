// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class THEULTIMATEHEIST_API IInteractable
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
		float GetInteractDuration();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
		bool CanInteract(AActor * Actor);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
		void Interact(AActor * Actor);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Interaction")
		void SetHighlight(bool Highlight);
};
