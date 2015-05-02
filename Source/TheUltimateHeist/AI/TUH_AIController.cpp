// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIPawn.h"
#include "TUH_AIController.h"

ATUH_AIController::ATUH_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ATUH_AIController::OnAIPerceptionUpdated(TArray<AActor*> Actors)
{
	if (IsKilled)
	{
		return;
	}

	auto AIPerception = GetPerceptionComponent();
	for (auto Actor : Actors)
	{
		FActorPerceptionBlueprintInfo Info;
		if (AIPerception->GetActorsPerception(Actor, Info))
		{
			for (auto Stimuli : Info.LastSensedStimuli)
			{
				auto Pawn = GetPawn();
				if (Pawn->GetClass()->ImplementsInterface(UAIPawn::StaticClass()))
				{
					auto ActivelySensedActors = IAIPawn::Execute_GetActivelySensedActors(Pawn);
					auto SensedActorsKeys = IAIPawn::Execute_GetSensedActorsKeys(Pawn);
					auto SensedActorsValues = IAIPawn::Execute_GetSensedActorsValues(Pawn);

					if (!Stimuli.IsExpired() && Stimuli.IsActive())
					{
						auto Sense = AIPerception->GetSenseConfig(Stimuli.Type)->GetSenseImplementation();
						auto SenseValue = IAIPawn::Execute_UpdateStimuli(Pawn, Actor, Stimuli.Strength, Stimuli.ReceiverLocation, Stimuli.StimulusLocation, Sense);
						if (SenseValue)
						{
							int Key = -1;
							if (!SensedActorsKeys.Contains(Actor))
							{
								Key = SensedActorsKeys.Add(Actor);
								SensedActorsValues.Add(0);
							}
							else
							{
								Key = SensedActorsKeys.IndexOfByKey(Actor);
							}

							SensedActorsValues[Key] += SenseValue;
							if (SensedActorsValues[Key] >= 100)
							{
								IAIPawn::Execute_DetectedActor(Pawn, Actor);
							}

							if (!ActivelySensedActors.Contains(Actor))
							{
								ActivelySensedActors.Add(Actor);
							}
						}
						else
						{
							ActivelySensedActors.Remove(Actor);
						}
					}
					else
					{
						ActivelySensedActors.Remove(Actor);
					}

					IAIPawn::Execute_SetActivelySensedActors(Pawn, ActivelySensedActors);
					IAIPawn::Execute_SetSensedActorsKeys(Pawn, SensedActorsKeys);
					IAIPawn::Execute_SetSensedActorsValues(Pawn, SensedActorsValues);
				}
			}
		}
	}
}

void ATUH_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsKilled)
	{
		return;
	}

	auto Pawn = GetPawn();
	if (Pawn->GetClass()->ImplementsInterface(UAIPawn::StaticClass()))
	{
		auto ActivelySensedActors = IAIPawn::Execute_GetActivelySensedActors(Pawn);
		auto SensedActorsKeys = IAIPawn::Execute_GetSensedActorsKeys(Pawn);
		auto SensedActorsValues = IAIPawn::Execute_GetSensedActorsValues(Pawn);
		TArray<int> KeysToRemove;
		for (auto Key = 0; Key < SensedActorsKeys.Num(); Key++)
		{
			auto Actor = SensedActorsKeys[Key];
			if (!ActivelySensedActors.Contains(Actor))
			{
				SensedActorsValues[Key] -= DeltaTime * 100;
				if (SensedActorsValues[Key] <= 0)
				{
					KeysToRemove.Add(Key);
				}
			}
		}

		for (auto i = KeysToRemove.Num() - 1; i >= 0; i--)
		{
			SensedActorsKeys.RemoveAt(KeysToRemove[i]);
			SensedActorsValues.RemoveAt(KeysToRemove[i]);
		}

		IAIPawn::Execute_SetSensedActorsKeys(Pawn, SensedActorsKeys);
		IAIPawn::Execute_SetSensedActorsValues(Pawn, SensedActorsValues);
	}
}

void ATUH_AIController::Killed()
{
	if (IsKilled)
	{
		return;
	}

	//PerceptionComponent->DestroyComponent();
	IsKilled = true;

	StopMovement();
	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Actor Killed"));
	}
	
	auto Pawn = GetPawn();
	if (Pawn->GetClass()->ImplementsInterface(UAIPawn::StaticClass()))
	{
		auto Blackboard = FindComponentByClass<UBlackboardComponent>();
		if (Blackboard)
		{
			Blackboard->SetValueAsBool(TEXT("Alerted"), false);
		}

		IAIPawn::Execute_SetSensedActorsKeys(Pawn, TArray<AActor *>());
		IAIPawn::Execute_SetSensedActorsValues(Pawn, TArray<float>());
	}
}