// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "TUH_GameMode.h"
#include "Levels/TUH_Level.h"
#include "TUH_GameState.h"
#include "../GameManagers/Objectives/Objective.h"

void ATUH_GameMode::StartMatch()
{
	Super::StartMatch();

	CurrentObjective = -1;
	NextObjective();
}

void ATUH_GameMode::SetObjective(UObjective * Objective)
{
	auto GameState = GetGameState<ATUH_GameState>();
	if (GameState)
	{
		GameState->SetCurrentObjective(Objective);
		Objective->OnCompleted.BindDynamic(this, &ATUH_GameMode::NextObjective);
		Objective->StartObjective();
	}
}

void ATUH_GameMode::NextObjective()
{
	CurrentObjective++;

	auto Level = Cast<ATUH_Level>(GetWorld()->GetCurrentLevel()->GetLevelScriptActor());
	if (Level)
	{
		if (CurrentObjective < Level->Objectives.Num())
		{
			auto Objective = Level->Objectives[CurrentObjective];
			SetObjective(Objective);
		}
		else
		{
			EndMatch();
		}
	}
}