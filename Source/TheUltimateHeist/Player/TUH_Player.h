#pragma once

#include "TheUltimateHeistCharacter.h"
#include "../Interfaces/Armed.h"

#include "TUH_Player.generated.h"

class AWeapon;

UCLASS()
class ATUH_Player : public ATheUltimateHeistCharacter, public IArmed
{
	GENERATED_BODY()

public:
	ATUH_Player(const FObjectInitializer & ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual FVector GetForwardVector() override;
	virtual FVector GetEyePosition() override;
	virtual USkeletalMeshComponent * GetSpecificPawnMesh(bool bFirstPerson) override;
	virtual FName GetWeaponAttachPoint(bool bFirstPerson) override;

protected:
	UPROPERTY(Transient, Replicated)
		TArray<AWeapon *> Inventory;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
		AWeapon * CurrentWeapon;
	UFUNCTION()
		void OnRep_CurrentWeapon(AWeapon * LastWeapon);

	void AddWeapon(AWeapon * Weapon);

	void EquipWeapon(AWeapon * Weapon);
	UFUNCTION(Reliable, Server, WithValidation)
		void SERVER_EquipWeapon(AWeapon * Weapon);
	bool SERVER_EquipWeapon_Validate(AWeapon * Weapon);
	void SERVER_EquipWeapon_Implementation(AWeapon * Weapon);

	void SetCurrentWeapon(AWeapon * NewWeapon, AWeapon * LastWeapon = nullptr);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void OnFire() override;
	void Deploy();
	void Interact();
};