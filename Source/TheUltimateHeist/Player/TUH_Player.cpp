#include "TheUltimateHeist.h"
#include "TUH_Player.h"
#include "../Items/Gun.h"

ATUH_Player::ATUH_Player(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CustomTagColor = FLinearColor(0.0F, 0.8F, 1.0F);
}

void ATUH_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		Gun = GetWorld()->SpawnActor<AGun>(AGun::StaticClass(), SpawnParams);
		Gun->OnEnterInventory(this);
	}
}

void ATUH_Player::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Deploy", IE_Pressed, this, &ATUH_Player::Deploy);
	InputComponent->BindAction("Interact", IE_Pressed, this, &ATUH_Player::Interact);
}

FVector ATUH_Player::GetForwardVector()
{
	return GetViewRotation().Vector();
}

FVector ATUH_Player::GetEyePosition()
{
	return GetPawnViewLocation();
}

void ATUH_Player::PositionGun()
{
	UE_LOG(TUHLog, Log, TEXT("[PositionGun] %s: %d"), LOG_NETMODE(), Gun);
	if (Gun)
	{
	}
}

void ATUH_Player::OnFire()
{
	UE_LOG(TUHLog, Log, TEXT("[OnFire] %s: %d"), LOG_NETMODE(), Gun);
	if (Gun)
	{
		Gun->Shoot();
	}
}

void ATUH_Player::Deploy()
{
	UE_LOG(TUHLog, Log, TEXT("[Deploy] %s: %d"), LOG_NETMODE(), Gun);
	if (Gun)
	{
		Gun->DrawGun();
	}
}

void ATUH_Player::Interact()
{
	UE_LOG(TUHLog, Log, TEXT("[Interact] %s: "), LOG_NETMODE());
}