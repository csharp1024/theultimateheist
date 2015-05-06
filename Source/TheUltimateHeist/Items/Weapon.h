// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AmmoTypes/Ammo.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponShoot);

USTRUCT()
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimationAsset * WeaponAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage * Anim1P;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage * Anim3P;
	UPROPERTY(EditDefaultsOnly, Category = "Timing")
		float Time;
};

UCLASS()
class THEULTIMATEHEIST_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer & ObjectInitializer);

	bool Shooting;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		TScriptInterface<class IArmed> MyPawn;
	UFUNCTION()
		void OnRep_MyPawn();
	void SetOwningPawn(const TScriptInterface<class IArmed> & Pawn);

	void OnEnterInventory(const TScriptInterface<class IArmed> & Pawn);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USkeletalMeshComponent * Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		FWeaponAnim EquipAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		FWeaponAnim ShootAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		FWeaponAnim ReloadAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		UParticleSystem * ShootParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue * ShootSound;

	UPROPERTY(EditDefaultsOnly, Category = "Features")
		float Accuracy;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		int32 Damage;
	UPROPERTY(EditDefaultsOnly, Category = "Features")
		TSubclassOf<AAmmo> AmmoType;

	void OnEquip();
	void OnUnEquip();

	void AttachMeshToPawn();
	void DetachMeshFromPawn();

	void Shoot();
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_Shoot();
	bool SERVER_Shoot_Validate();
	void SERVER_Shoot_Implementation();
	void ShootFinished();

	UFUNCTION(NetMulticast, Unreliable)
		void MULTICAST_Shoot();
	void MULTICAST_Shoot_Implementation();

	void SetMesh(USkeletalMesh * SkeletalMesh);

	void PlayWeaponMontage(FWeaponAnim & Anim);
};
