// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "GunDamageType.h"

void UGunDamageType::ApplyDamage(AActor * Pawn, int Damage, const FHitResult & Hit, AActor * Owner)
{
	auto Dir = Hit.TraceStart - Hit.TraceEnd;
	Dir.Normalize();
	UGameplayStatics::ApplyPointDamage(Pawn, Damage, Dir, Hit, Owner->GetInstigatorController(), Owner, StaticClass());
}