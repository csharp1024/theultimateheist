// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ammo.generated.h"

UCLASS()
class UAmmo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		uint32 NumShots;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		float Accuracy;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		uint32 bPenetrates : 1;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		uint32 Damage;

	virtual void HandleShot(AActor * Owner, const FVector & Start, const FVector & Dir);
};
