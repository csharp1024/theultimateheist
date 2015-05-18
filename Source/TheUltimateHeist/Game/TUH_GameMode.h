// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TUH_GameMode.generated.h"

class UObjective;

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_GameMode : public AGameMode
{
	GENERATED_BODY()

protected:
	int CurrentObjective;
	
public:
	virtual void StartMatch() override;

	void SetObjective(UObjective * Objective);
	UFUNCTION()
		void NextObjective();
};
