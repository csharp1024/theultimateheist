#pragma once

#include "Ammo.h"
#include "Bullet.generated.h"

UCLASS()
class ABullet : public AAmmo
{
	GENERATED_BODY()

public:
	virtual void LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End) override;
};