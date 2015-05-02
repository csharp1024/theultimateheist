// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "AI/AIPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TUH_HUD.h"

void ATUH_HUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DetectionMaterialDynamic = UMaterialInstanceDynamic::Create(DetectionMaterial, this);
	InteractionMaterialDynamic = UMaterialInstanceDynamic::Create(InteractionMaterial, this);
}

void ATUH_HUD::DrawHUD()
{
	HalfX = Canvas->ClipX / 2;
	HalfY = Canvas->ClipY / 2;

	DrawIndicators();

	DrawCrosshair();
	DrawDetectionScale();
	DrawInteractionScale();
}

void ATUH_HUD::Tick(float DeltaTime)
{
	float MaxDetection = 0;

	auto Pawn = GetOwningPawn();
	if (Pawn)
	{
		TArray<AActor *> Actors;
		UGameplayStatics::GetAllActorsWithInterface(Pawn, UAIPawn::StaticClass(), Actors);
		for (auto Actor : Actors)
		{
			auto SensedKeys = IAIPawn::Execute_GetSensedActorsKeys(Actor);
			auto SensedValues = IAIPawn::Execute_GetSensedActorsValues(Actor);

			for (auto i = 0; i < SensedKeys.Num(); i++)
			{
				auto Key = SensedKeys[i];
				auto Value = SensedValues[i];
				if (Key == Pawn)
				{
					if (Value > MaxDetection)
					{
						MaxDetection = Value;
					}
				}
			}
		}
	}

	DetectionMaterialDynamic->SetScalarParameterValue(TEXT("DetectionAmount"), MaxDetection / 100);
	InteractionMaterialDynamic->SetScalarParameterValue(TEXT("InteractionAmount"), 0);
}

void ATUH_HUD::DrawIndicators()
{
	auto Pawn = GetOwningPawn();
	if (Pawn)
	{
		TArray<AActor *> Actors;
		UGameplayStatics::GetAllActorsWithInterface(Pawn, UAIPawn::StaticClass(), Actors);
		for (auto Actor : Actors)
		{
			auto SensedKeys = IAIPawn::Execute_GetSensedActorsKeys(Actor);
			auto SensedValues = IAIPawn::Execute_GetSensedActorsValues(Actor);

			auto Blackboard = Actor->GetInstigatorController()->FindComponentByClass<UBlackboardComponent>();
			if (Blackboard && Blackboard->GetValueAsBool(TEXT("Alerted")))
			{
				DrawIndicator(Actor, DetectedColor, ExclamationIcon);
			}
			else if (Blackboard && Blackboard->GetValueAsBool(TEXT("Phoning")))
			{
				DrawIndicator(Actor, DetectedColor, PhoneIcon);
			}
			else if (SensedKeys.Num() > 0)
			{
				DrawIndicator(Actor, InterestColor, QuestionIcon);
			}
		}
	}
}

void ATUH_HUD::DrawCrosshair()
{
	const auto CrosshairSize = 16;

	DrawTexture(
		Crosshair,
		HalfX - CrosshairSize / 2, HalfY - CrosshairSize / 2,
		CrosshairSize, CrosshairSize,
		0, 0,
		1, 1
	);
}

void ATUH_HUD::DrawDetectionScale()
{
	const auto IndicatorSize = 128;

	DrawMaterial(
		DetectionMaterialDynamic,
		HalfX - IndicatorSize / 2, HalfY - IndicatorSize / 2,
		IndicatorSize, IndicatorSize,
		0, 0,
		1, 1,
		1, false,
		90, FVector2D(0.5f, 0.5f)
	);
}

void ATUH_HUD::DrawInteractionScale()
{
	const auto IndicatorSize = 128;

	DrawMaterial(
		InteractionMaterialDynamic,
		HalfX - IndicatorSize / 2, HalfY - IndicatorSize / 2,
		IndicatorSize, IndicatorSize,
		0, 0,
		1, 1,
		1, false,
		0, FVector2D(0.5f, 0.5f)
	);
}

void ATUH_HUD::DrawIndicator(const AActor * Actor, const FLinearColor & Color, UTexture2D * Icon)
{
	const auto IconSize = 32;
	const auto CenterDistance = 128;

	auto Pawn = GetOwningPawn();
	if (Pawn)
	{
		auto HeadLocation = Actor->GetActorLocation() + FVector(0, 0, Cast<APawn>(Actor)->BaseEyeHeight + 20);

		auto ScreenPosition = Project(HeadLocation);
		auto AdjustedScreenPosition = ScreenPosition - FVector(HalfX, HalfY, 0);
		auto IsBehind = AdjustedScreenPosition.Z == 0;
		auto AbsAdjustedScreenPosition = FVector(FMath::Abs(AdjustedScreenPosition.X), FMath::Abs(AdjustedScreenPosition.Y), 0);
		if (FMath::Abs(AdjustedScreenPosition.X) > HalfX || FMath::Abs(AdjustedScreenPosition.Y) > HalfY || IsBehind)
		{
			FVector RelativeVector;
			if (IsBehind)
			{
				auto ActorDiff = HeadLocation - Pawn->GetActorLocation();
				RelativeVector = Pawn->GetActorRotation().UnrotateVector(ActorDiff);
				RelativeVector = RelativeVector.RotateAngleAxis(-90, FVector::UpVector);
				RelativeVector.Normalize();
				RelativeVector *= CenterDistance;
			}
			else
			{
				auto MaxDirection = FMath::Max(AbsAdjustedScreenPosition.X - HalfX, AbsAdjustedScreenPosition.Y - HalfY);
				auto MinDirection = FMath::Min(HalfX, HalfY);
				RelativeVector = AdjustedScreenPosition;
				RelativeVector.Normalize();
				RelativeVector *= FMath::Clamp<float>(MinDirection - MaxDirection, 0, MinDirection) + CenterDistance;
			}

			ScreenPosition = RelativeVector + FVector(HalfX, HalfY, 0);

			auto Dir = RelativeVector;
			Dir.Normalize();
			Dir *= IconSize;

			DrawTexture(
				Arrow,
				ScreenPosition.X + Dir.X - IconSize / 2, ScreenPosition.Y + Dir.Y - IconSize / 2,
				IconSize, IconSize,
				0, 0,
				1, 1,
				Color,
				BLEND_Translucent,
				1, false,
				RelativeVector.Rotation().Yaw + 90, FVector2D(0.5f, 0.5f)
			);
		}

		DrawTexture(
			Icon,
			ScreenPosition.X - IconSize / 2, ScreenPosition.Y - IconSize / 2,
			IconSize, IconSize,
			0, 0,
			1, 1,
			Color
		);
	}
}