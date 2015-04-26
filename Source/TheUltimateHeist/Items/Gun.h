// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Gun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGunShoot);

UCLASS()
class THEULTIMATEHEIST_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun(const FObjectInitializer & ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const;

	UPROPERTY(BlueprintReadOnly)
		float Time;
	UPROPERTY(BlueprintReadOnly)
		bool Shooting;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armed)
		bool Armed;

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
		FGunShoot OnShoot;

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
		void DrawGun();
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_DrawGun();
	bool SERVER_DrawGun_Validate();
	void SERVER_DrawGun_Implementation();

	UFUNCTION()
		void OnRep_Armed();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetMesh(USkeletalMesh * SkeletalMesh);
};
