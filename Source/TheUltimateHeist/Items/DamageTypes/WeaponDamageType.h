// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "WeaponDamageType.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API UWeaponDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	virtual void ApplyDamage(AActor * Pawn, int Damage, const FHitResult & Hit, AActor * Owner) PURE_VIRTUAL(UWeaponDamageType::ApplyDamage, );
};
