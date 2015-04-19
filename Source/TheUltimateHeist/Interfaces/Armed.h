// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Armed.generated.h"

UINTERFACE(MinimalAPI)
class UArmed : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class THEULTIMATEHEIST_API IArmed
{
	GENERATED_IINTERFACE_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Armed")
		FVector GetForwardVector();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Armed")
		FVector GetEyePosition();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Armed")
		void PositionGun();
};