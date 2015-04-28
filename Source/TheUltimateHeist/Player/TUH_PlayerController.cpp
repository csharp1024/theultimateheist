// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "../TUH_GameState.h"
#include "UnrealNetwork.h"
#include "AI/AIPawn.h"
#include "TUH_PlayerController.h"

void ATUH_PlayerController::HighlightManager_AddActor(AActor * Actor, float Length)
{
	if (HasAuthority())
	{
		auto GameState = Cast<ATUH_GameState>(GetWorld()->GameState);
		if (GameState->HighlightManager)
		{
			GameState->HighlightManager->SERVER_AddActor(Actor, Length);
		}
	}
	else
	{
		SERVER_HighlightManager_AddActor(Actor, Length);
	}
}

bool ATUH_PlayerController::SERVER_HighlightManager_AddActor_Validate(AActor * Actor, float Length)
{
	return Actor->GetClass()->ImplementsInterface(UAIPawn::StaticClass());
}

void ATUH_PlayerController::SERVER_HighlightManager_AddActor_Implementation(AActor * Actor, float Length)
{
	HighlightManager_AddActor(Actor, Length);
}