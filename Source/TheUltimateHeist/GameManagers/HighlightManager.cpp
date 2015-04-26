// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Player/TUH_PlayerController.h"
#include "AI/AIPawn.h"
#include "HighlightManager.h"

AHighlightManager::AHighlightManager(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

void AHighlightManager::SERVER_AddActor(AActor * Actor, float Length)
{
	if (HasAuthority())
	{
		auto Index = HighlightedActors.AddUnique(Actor);
		if (Index >= HighlightedCounters.Num())
		{
			HighlightedCounters.SetNum(Index + 1);
		}
		HighlightedCounters[Index] = Length;

		IAIPawn::Execute_SetHighlight(Actor, true);
	}
}

void AHighlightManager::UpdateHighlights(float DeltaTime)
{
	if (HasAuthority())
	{
		TArray<int> KeysToRemove;
		for (auto i = 0; i < HighlightedCounters.Num(); i++)
		{
			HighlightedCounters[i] -= DeltaTime;

			auto Counter = HighlightedCounters[i];
			if (Counter <= 0)
			{
				KeysToRemove.Add(i);
			}
		}

		for (auto i = KeysToRemove.Num() - 1; i >= 0; i--)
		{
			if (HighlightedActors[i]->GetClass()->ImplementsInterface(UAIPawn::StaticClass()))
			{
				IAIPawn::Execute_SetHighlight(HighlightedActors[i], false);
			}
			HighlightedCounters.RemoveAt(i);
			HighlightedActors.RemoveAt(i);
		}
	}
}