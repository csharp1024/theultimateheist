// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "InteractionObjective.h"

#include "../../Items/Interactable.h"

void UInteractionObjective::StartObjective()
{
	Interactable->OnInteract.BindDynamic(this, &UInteractionObjective::CompleteObjective);
}