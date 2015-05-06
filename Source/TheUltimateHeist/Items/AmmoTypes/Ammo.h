// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ammo.generated.h"

UCLASS(BlueprintType, Blueprintable)
class AAmmo : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		int32 NumShots;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		bool bPenetrates;

	virtual void LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End);

protected:
	AActor * Owner;
	int32 Damage;
};
