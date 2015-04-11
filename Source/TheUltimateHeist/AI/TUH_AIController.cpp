// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISense_Hearing.h"
//#include "Perception/AISenseConfig_Hearing.h"
//#include "Perception/AISense_Damage.h"
//#include "Perception/AISenseConfig_Damage.h"
#include "TUH_AICharacter.h"
#include "AICharacterInterface.h"
#include "TUH_AIController.h"

ATUH_AIController::ATUH_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPerception = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, "AIPerception");
	AddComponent(FName("AIPerception"), true, FTransform(), AIPerception);
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &ATUH_AIController::OnAIPerceptionUpdated);

	PrimaryActorTick.bCanEverTick = true;
}

void ATUH_AIController::OnAIPerceptionUpdated(TArray<AActor*> Actors)
{
	for (auto Actor : Actors)
	{
		FActorPerceptionBlueprintInfo Info;
		if (AIPerception->GetActorsPerception(Actor, Info))
		{
			for (auto Stimuli : Info.LastSensedStimuli)
			{
				auto Pawn = Cast<ATUH_AICharacter>(GetPawn());
				if (Pawn)
				{
					if (!Stimuli.IsExpired() && Stimuli.IsActive())
					{
						auto Sense = AIPerception->GetSenseConfig(Stimuli.Type)->GetSenseImplementation();
						auto SenseValue = Pawn->UpdateStimuli(Actor, Stimuli.ReceiverLocation, Stimuli.StimulusLocation, Sense);
						if (SenseValue)
						{
							int Key = -1;
							if (!Pawn->SensedActorsKeys.Contains(Actor))
							{
								Key = Pawn->SensedActorsKeys.Add(Actor);
								Pawn->SensedActorsValues.Add(0);
							}
							else
							{
								Key = Pawn->SensedActorsKeys.IndexOfByKey(Actor);
							}
							Pawn->SensedActorsValues[Key] += SenseValue;
							if (Pawn->SensedActorsValues[Key] >= 100)
							{
								Pawn->DetectedActor(Actor);
							}

							if (!Pawn->ActivelySensedActors.Contains(Actor))
							{
								Pawn->ActivelySensedActors.Add(Actor);
							}
						}
						else
						{
							Pawn->ActivelySensedActors.Remove(Actor);
						}
					}
					else
					{
						Pawn->ActivelySensedActors.Remove(Actor);
					}
				}
			}
		}
	}
}

void ATUH_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Pawn = Cast<ATUH_AICharacter>(GetPawn());
	if (Pawn)
	{
		TArray<int> KeysToRemove;
		for (auto Actor : Pawn->SensedActorsKeys)
		{
			auto Key = Pawn->SensedActorsKeys.IndexOfByKey(Actor);
			if (!Pawn->ActivelySensedActors.Contains(Actor))
			{
				Pawn->SensedActorsValues[Key] -= DeltaTime * 100;
				if (Pawn->SensedActorsValues[Key] <= 0)
				{
					KeysToRemove.Add(Key);
				}
			}
		}

		for (auto i = KeysToRemove.Num() - 1; i >= 0; i--)
		{
			Pawn->SensedActorsKeys.RemoveAt(KeysToRemove[i]);
			Pawn->SensedActorsValues.RemoveAt(KeysToRemove[i]);
		}
	}
}
