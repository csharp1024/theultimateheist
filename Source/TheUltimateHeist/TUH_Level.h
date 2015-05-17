// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/LevelScriptActor.h"
#include "TUH_Level.generated.h"

class UObjective;

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_Level : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
		TArray<UObjective *> Objectives;
};
