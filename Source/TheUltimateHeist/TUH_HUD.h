// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "TUH_HUD.generated.h"

/**
 * 
 */
UCLASS()
class THEULTIMATEHEIST_API ATUH_HUD : public AHUD
{
	GENERATED_BODY()

protected:
	UMaterialInstanceDynamic * DetectionMaterialDynamic;
	UMaterialInstanceDynamic * InteractionMaterialDynamic;

	void DrawCrosshair();
	void DrawDetectionScale();
	void DrawInteractionScale();
	void DrawIndicators();

	void DrawIndicator(const AActor * Actor, const FLinearColor & Color, UTexture2D * Icon);

	float HalfX;
	float HalfY;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
		UMaterial * DetectionMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "Materials")
		UMaterial * InteractionMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Textures")
		UTexture2D * Crosshair;
	UPROPERTY(EditDefaultsOnly, Category = "Textures")
		UTexture2D * QuestionIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Textures")
		UTexture2D * ExclamationIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Textures")
		UTexture2D * PhoneIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Textures")
		UTexture2D * Arrow;

	UPROPERTY(EditDefaultsOnly, Category = "Colors")
		FLinearColor InterestColor;
	UPROPERTY(EditDefaultsOnly, Category = "Colors")
		FLinearColor DetectedColor;

	virtual void PostInitializeComponents() override;

	virtual void DrawHUD() override;

	virtual void Tick(float DeltaTime) override;
};
