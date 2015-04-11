// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "UnrealNetwork.h"
#include "TUH_AICharacter.h"

void ATUH_AICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATUH_AICharacter, SensedActorsKeys);
	DOREPLIFETIME(ATUH_AICharacter, SensedActorsValues);
}