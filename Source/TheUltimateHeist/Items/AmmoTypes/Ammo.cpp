// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "Ammo.h"

void AAmmo::LaunchProjectile(AActor * Owner, int32 Damage, const FVector & Start, const FVector & End)
{
	this->Owner = Owner;
	this->Damage = Damage;
}
