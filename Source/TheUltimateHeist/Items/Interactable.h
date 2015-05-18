// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnInteract);

UCLASS()
class AInteractable : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
		float InteractDuration;
	UPROPERTY()
		FOnInteract OnInteract;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		bool CanInteract(AActor * Actor);
	bool CanInteract_Implementation(AActor * Actor);
	void Interact(AActor * Actor);
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Interacted(AActor * Actor);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
		void SetHighlight(bool Highlight);
	void SetHighlight_Implementation(bool Highlight);
};
