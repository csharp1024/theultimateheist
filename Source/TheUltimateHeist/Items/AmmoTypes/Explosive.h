#pragma once

#include "Ammo.h"
#include "Explosive.generated.h"

UCLASS()
class AExplosive : public AAmmo
{
	GENERATED_BODY()

public:
	AExplosive();

	UPROPERTY(EditDefaultsOnly, Category = Fixed)
		float Offset;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float Speed;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float BlastRadius;
	UPROPERTY(EditDefaultsOnly, Category = Effects)
		UParticleSystem * ExplosionEffect;

	virtual void LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End) override;

	virtual void Tick(float DeltaTime) override;

protected:
	FVector Direction;
};