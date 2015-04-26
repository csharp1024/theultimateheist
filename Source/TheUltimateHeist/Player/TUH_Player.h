#pragma once

#include "TheUltimateHeistCharacter.h"
#include "../Interfaces/Armed.h"

#include "TUH_Player.generated.h"

UCLASS()
class ATUH_Player : public ATheUltimateHeistCharacter, public IArmed
{
	GENERATED_BODY()

public:
	ATUH_Player(const FObjectInitializer & ObjectInitializer);

	virtual void PostInitializeComponents() override;

	virtual FVector GetForwardVector() override;
	virtual FVector GetEyePosition() override;
	virtual void PositionGun() override;

protected:
	class AGun * Gun;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	virtual void OnFire() override;
	void Deploy();
	void Interact();
};