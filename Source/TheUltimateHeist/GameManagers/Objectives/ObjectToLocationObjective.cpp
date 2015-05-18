// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "ObjectToLocationObjective.h"

#include "Engine/TriggerBase.h"

void UObjectToLocationObjective::StartObjective()
{
	Trigger->OnActorBeginOverlap.AddDynamic(this, &UObjectToLocationObjective::OnOverlap);
}

void UObjectToLocationObjective::OnOverlap(AActor * OtherActor)
{
	if (OtherActor == Object)
	{
		CompleteObjective();
	}
}