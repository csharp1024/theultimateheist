#include "TheUltimateHeist.h"

#include "Explosive.h"

#define COLLISION_EXPLOSIVE		ECC_GameTraceChannel1

AExplosive::AExplosive()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosive::LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End)
{
	Super::LaunchProjectile(Owner, Damage, Start, End);

	Direction = End - Start;
	Direction.Normalize();

	SetActorLocation(Start + Direction * Offset);
}

void AExplosive::Tick(float DeltaTime)
{
	FHitResult Result;
	SetActorLocation(GetActorLocation() + Direction * Speed * DeltaTime, true, &Result);
	if (Result.Actor.IsValid())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			ExplosionEffect,
			Result.ImpactPoint
			);

		TArray<AActor *> IgnoreActors;
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			this,
			Damage,
			0,
			Result.ImpactPoint,
			BlastRadius * 0.8f,
			BlastRadius * 1.2f,
			0,
			UDamageType::StaticClass(),
			IgnoreActors,
			this,
			Owner->GetInstigatorController(),
			ECollisionChannel::ECC_WorldStatic
			);
		Destroy();
	}
}