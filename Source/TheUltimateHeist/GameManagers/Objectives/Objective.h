// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Objective.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnCompleted);

/**
 * 
 */
UCLASS(EditInlineNew)
class THEULTIMATEHEIST_API UObjective : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		FString Name;
	UPROPERTY(EditDefaultsOnly)
		FString Description;

	UPROPERTY(EditDefaultsOnly, Instanced)
		TArray<UObjective *> SubObjectives;

	UPROPERTY()
		FOnCompleted OnCompleted;

	virtual void StartObjective();
	UFUNCTION()
		void CompleteObjective();
};
