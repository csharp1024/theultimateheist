// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Objective.h"

void UObjective::StartObjective()
{
}

void UObjective::CompleteObjective()
{
	OnCompleted.ExecuteIfBound();
}