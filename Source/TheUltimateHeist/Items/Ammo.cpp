// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Ammo.h"

void UAmmo::HandleShot(AActor * Owner, const FVector & Start, const FVector & Dir)
{
	for (auto i = 0U; i < NumShots; i++)
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
		auto World = GetWorld();
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

		for (auto Hit : OutHits)
		{
			UKismetSystemLibrary::DrawDebugPoint(World, Hit.ImpactPoint, 10.f, Hit.bBlockingHit ? FColor::Red : FColor::Green, Lifetime);

			if (Hit.Actor.IsValid())
			{
				UE_LOG(TUHLog, Log, TEXT("Hit %s: %s [%s]"), *(Hit.Actor->GetName()), *Hit.Component->GetName(), *Hit.BoneName.ToString());
				auto Pawn = Cast<APawn>(Hit.Actor.Get());
				if (Pawn)
				{
					if (Hit.BoneName != NAME_None)
					{
						UGameplayStatics::ApplyPointDamage(Pawn, Damage, -Dir, Hit, Owner->GetInstigatorController(), Owner, UDamageType::StaticClass());
					}
				}
			}
		}
	}
}