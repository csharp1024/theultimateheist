// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISense_Hearing.h"
//#include "Perception/AISenseConfig_Hearing.h"
//#include "Perception/AISense_Damage.h"
//#include "Perception/AISenseConfig_Damage.h"
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
				if (!Stimuli.IsExpired() && Stimuli.IsActive())
				{
					auto Pawn = GetPawn();
					if (Pawn->GetClass()->ImplementsInterface(UAICharacterInterface::StaticClass()))
					{
						auto Sense = AIPerception->GetSenseConfig(Stimuli.Type)->GetSenseImplementation();
						auto SenseValue = IAICharacterInterface::Execute_UpdateStimuli(Pawn, Actor, Stimuli.ReceiverLocation, Stimuli.StimulusLocation, Sense);
						if (SenseValue)
						{
							if (!SensedActors.Contains(Actor))
							{
								SensedActors.Add(Actor) = 0;
							}
							SensedActors[Actor] += SenseValue;

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
				}
				else
				{
					ActivelySensedActors.Remove(Actor);
				}
			}
		}
	}
}

void ATUH_AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto Pair : SensedActors)
	{
		auto Actor = Pair.Key;
		if (!ActivelySensedActors.Contains(Actor))
		{
			SensedActors[Actor] -= DeltaTime * 100;
			if (SensedActors[Actor] <= 0)
			{
				SensedActors.Remove(Actor);
			}
		}
	}
}

TArray<AActor *> ATUH_AIController::GetSensedActors(TArray<float> & AlertedValues)
{
	AlertedValues.Reset();
	SensedActors.GenerateValueArray(AlertedValues);

	TArray<AActor *> ActorArray;
	SensedActors.GenerateKeyArray(ActorArray);
	return ActorArray;
}
