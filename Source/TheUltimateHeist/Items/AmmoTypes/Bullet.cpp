#include "TheUltimateHeist.h"
#include "GenericTeamAgentInterface.h"
#include "Bullet.h"

void ABullet::LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End)
{
	Super::LaunchProjectile(Owner, Damage, Start, End);

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

	auto World = Owner->GetWorld();
	auto bHit = World->LineTraceMulti(
		OutHits,
		Start,
		End,
		Params,
		ObjectParams
		);

	auto Direction = End - Start;
	Direction.Normalize();

	bool Stop = false;
	for (auto Hit : OutHits)
	{
		if (Hit.Actor.IsValid())
		{
			auto MeshComponent = Cast<UMeshComponent>(Hit.Component.Get());
			if (MeshComponent)
			{
				auto Pawn = Cast<APawn>(Hit.Actor.Get());
				if (Pawn)
				{
					if (Pawn->Tags.Contains(TEXT("Destructable")))
					{
						UGameplayStatics::ApplyPointDamage(Pawn, Damage, -Direction, Hit, Owner->GetInstigatorController(), this, UDamageType::StaticClass());
						if (!bPenetrates)
						{
							Stop = true;
						}
					}
					else if (FGenericTeamId::GetAttitude(Pawn, Owner) != ETeamAttitude::Friendly)
					{
						if (Hit.BoneName != NAME_None)
						{
							UGameplayStatics::ApplyPointDamage(Pawn, Damage, -Direction, Hit, Owner->GetInstigatorController(), this, UDamageType::StaticClass());
							if (!bPenetrates)
							{
								Stop = true;
							}
						}
					}
				}
				else if (!bPenetrates)
				{
					Stop = true;
				}

				if (Stop)
				{
					// spawn decal
				}
			}
		}

		if (Stop)
		{
			break;
		}
	}

	Destroy();
}