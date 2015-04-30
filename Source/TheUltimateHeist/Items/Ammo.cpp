// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "Ammo.h"

void UAmmo::HandleShot(AActor * Owner, const FVector & Start, const FVector & Dir, float Accuracy, int32 Damage)
{
	for (auto i = 0; i < NumShots; i++)
	{
		TArray<FHitResult> OutHits;
		FCollisionQueryParams Params;
		Params.bReturnPhysicalMaterial = true;
		Params.bTraceAsyncScene = true;
		Params.AddIgnoredActor(Owner);

		FCollisionObjectQueryParams ObjectParams;
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

		auto InaccuracyCone = FMath::VRandCone(Dir, FMath::DegreesToRadians((1 - Accuracy) * 30));
		auto End = Start + InaccuracyCone * 10000;
		auto World = Owner->GetWorld();
		auto bHit = World->LineTraceMulti(
			OutHits,
			Start,
			End,
			Params,
			ObjectParams
			);
		auto Lifetime = 20.f;
		if (bHit && OutHits.Last().bBlockingHit)
		{
			auto BlockingHitPoint = OutHits.Last().ImpactPoint;
			UKismetSystemLibrary::DrawDebugLine(World, Start, BlockingHitPoint, FLinearColor::Red, Lifetime);
			UKismetSystemLibrary::DrawDebugLine(World, BlockingHitPoint, End, FLinearColor::Green, Lifetime);
		}
		else
		{
			UKismetSystemLibrary::DrawDebugLine(World, Start, End, FLinearColor::Red, Lifetime);
		}

		bool Stop = false;
		for (auto Hit : OutHits)
		{
			UKismetSystemLibrary::DrawDebugPoint(World, Hit.ImpactPoint, 10.f, Hit.bBlockingHit ? FColor::Red : FColor::Green, Lifetime);

			if (Hit.Actor.IsValid())
			{
				auto Pawn = Cast<APawn>(Hit.Actor.Get());
				if (Pawn)
				{
					if (FGenericTeamId::GetAttitude(Pawn, Owner) != ETeamAttitude::Friendly)
					{
						UE_LOG(TUHLog, Log, TEXT("Hit %s: %s [%s]"), *(Hit.Actor->GetName()), *Hit.Component->GetName(), *Hit.BoneName.ToString());
						if (Hit.BoneName != NAME_None)
						{
							DamageType.GetDefaultObject()->ApplyDamage(Pawn, Damage, Hit, Owner);
							if (!bPenetrates)
							{
								Stop = true;
							}
						}
					}
				}
			}

			if (Stop)
			{
				return;
			}
		}
	}
}