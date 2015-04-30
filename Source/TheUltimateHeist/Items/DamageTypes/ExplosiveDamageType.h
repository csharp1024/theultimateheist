// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponDamageType.h"
#include "ExplosiveDamageType.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API UExplosiveDamageType : public UWeaponDamageType
{
	GENERATED_BODY()
	
public:
	virtual void ApplyDamage(AActor * Pawn, int Damage, const FHitResult & Hit, AActor * Owner) override;
};
