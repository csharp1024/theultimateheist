// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TUH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Game State|Highlight Manager")
		void HighlightManager_AddActor(AActor * Actor, float Length = 10);
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_HighlightManager_AddActor(AActor * Actor, float Length);
	bool SERVER_HighlightManager_AddActor_Validate(AActor * Actor, float Length);
	void SERVER_HighlightManager_AddActor_Implementation(AActor * Actor, float Length);
};
