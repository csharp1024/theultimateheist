// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageTypes/WeaponDamageType.h"
#include "Ammo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class UAmmo : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		int32 NumShots;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		bool bPenetrates;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		TSubclassOf<UWeaponDamageType> DamageType;

	virtual void HandleShot(AActor * Owner, const FVector & Start, const FVector & Dir, float Accuracy, int32 Damage);
};
