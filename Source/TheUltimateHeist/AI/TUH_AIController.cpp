// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
//#include "Perception/AISense_Hearing.h"
//#include "Perception/AISenseConfig_Hearing.h"
//#include "Perception/AISense_Damage.h"
//#include "Perception/AISenseConfig_Damage.h"
#include "TUH_AIController.h"

ATUH_AIController::ATUH_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIPerception = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, "AIPerception");
	AddComponent(FName("AIPerception"), true, FTransform(), AIPerception);
	{
		auto Sense = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(AIPerception, "AISenseConfig_Sight");
		AIPerception->ConfigureSense(*Sense);
	}
	{
		//auto Sense = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Hearing>(AIPerception, "AISenseConfig_Hearing"),
		//AIPerception->ConfigureSense(*Sense);
	}
	{
		//auto Sense = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Damage>(AIPerception, "AISenseConfig_Damage")
		//AIPerception->ConfigureSense(*Sense);
	};
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &ATUH_AIController::OnAIPerceptionUpdated);
}

void ATUH_AIController::OnAIPerceptionUpdated(TArray<AActor*> Actors)
{
	SensedActors.Reset();
	for (auto Actor : Actors)
	{
		FActorPerceptionBlueprintInfo Info;
		if (AIPerception->GetActorsPerception(Actor, Info))
		{
			auto NotExpired = false;
			for (auto Stimuli : Info.LastSensedStimuli)
			{
				if (!Stimuli.IsExpired())
				{
					NotExpired = true;
				}
			}

			if (NotExpired)
			{
				SensedActors.Add(Actor);
			}
		}
	}
}
