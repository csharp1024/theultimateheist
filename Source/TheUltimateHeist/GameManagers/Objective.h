// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Info.h"
#include "Objective.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class THEULTIMATEHEIST_API UObjective : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		UObjective * NextObjective;
};
