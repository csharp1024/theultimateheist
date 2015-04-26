// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponShoot);

UCLASS()
class THEULTIMATEHEIST_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer & ObjectInitializer);

	UPROPERTY(BlueprintReadOnly)
		float Time;
	UPROPERTY(BlueprintReadOnly)
		bool Shooting;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		TScriptInterface<class IArmed> MyPawn;
	UFUNCTION()
		void OnRep_MyPawn();
	void SetOwningPawn(const TScriptInterface<class IArmed> & Pawn);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void OnEnterInventory(const TScriptInterface<class IArmed> & Pawn);

	UPROPERTY()
		USkeletalMeshComponent * Mesh;

	UPROPERTY(BlueprintAssignable)
		FWeaponShoot OnShoot;

	void OnEquip();
	void OnUnEquip();

	void AttachMeshToPawn();
	void DetachMeshFromPawn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void Shoot();
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_Shoot();
	bool SERVER_Shoot_Validate();
	void SERVER_Shoot_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
		void MULTICAST_Shoot();
	void MULTICAST_Shoot_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetMesh(USkeletalMesh * SkeletalMesh);
};
