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
	virtual FVector GetForwardVector() PURE_VIRTUAL(IArmed::GetForwardVector, return FVector(););
	virtual FVector GetEyePosition() PURE_VIRTUAL(IArmed::GetEyePosition, return FVector(););
	virtual USkeletalMeshComponent * GetSpecificPawnMesh(bool bFirstPerson) PURE_VIRTUAL(IArmed::GetSpecificPawnMesh, return nullptr;);
	virtual FName GetWeaponAttachPoint(bool bFirstPerson) PURE_VIRTUAL(IArmed::GetWeaponAttachPoint, return TEXT("Invalid"););
};