// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "TheUltimateHeistHUD.generated.h"

UCLASS()
class ATheUltimateHeistHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATheUltimateHeistHUD(const FObjectInitializer& ObjectInitializer);

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

