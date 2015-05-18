#include "TheUltimateHeist.h"
#include "TUH_Player.h"
#include "../Items/Weapon.h"
#include "UnrealNetwork.h"
#include "../Items/Interactable.h"

ATUH_Player::ATUH_Player(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	GetMesh()->bOwnerNoSee = true;
	GetMesh()->CustomTagColor = FLinearColor(0.0F, 0.8F, 1.0F);
}

void ATUH_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ATUH_Player, Inventory, COND_OwnerOnly);

	DOREPLIFETIME(ATUH_Player, CurrentWeapon);
}

void ATUH_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		SpawnDefaultInventory();
	}
}

void ATUH_Player::SpawnDefaultInventory()
{
	if (!HasAuthority())
	{
		return;
	}

	for (auto Class : DefaultInventoryClasses)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoCollisionFail = true;
		auto Weapon = GetWorld()->SpawnActor<AWeapon>(Class, SpawnParams);
		AddWeapon(Weapon);
	}
}

void ATUH_Player::AddWeapon(AWeapon * Weapon)
{
	if (HasAuthority())
	{
		Weapon->OnEnterInventory(this);
		Inventory.AddUnique(Weapon);
	}
}

void ATUH_Player::OnRep_CurrentWeapon(AWeapon * LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
}

void ATUH_Player::EquipWeapon(AWeapon * Weapon)
{
	if (Weapon)
	{
		if (HasAuthority())
		{
			SetCurrentWeapon(Weapon);
		}
		else
		{
			SERVER_EquipWeapon(Weapon);
		}
	}
}

bool ATUH_Player::SERVER_EquipWeapon_Validate(AWeapon * Weapon)
{
	return true;
}

void ATUH_Player::SERVER_EquipWeapon_Implementation(AWeapon * Weapon)
{
	EquipWeapon(Weapon);
}

void ATUH_Player::SetCurrentWeapon(AWeapon * NewWeapon, AWeapon * LastWeapon)
{
	AWeapon * LocalLastWeapon = nullptr;

	if (LastWeapon != nullptr)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip();
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

USkeletalMeshComponent * ATUH_Player::GetSpecificPawnMesh(bool bFirstPerson)
{
	if (bFirstPerson)
	{
		return GetMesh1P();
	}
	else
	{
		return GetMesh();
	}
}

FName ATUH_Player::GetWeaponAttachPoint(bool bFirstPerson)
{
	if (bFirstPerson)
	{
		return TEXT("b_RightWeaponSocket");
	}
	else
	{
		return TEXT("ik_hand_rSocket");
	}
}

void ATUH_Player::OnFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Shoot();
	}
}

void ATUH_Player::Deploy()
{
	EquipWeapon(Inventory[0]);
}

void ATUH_Player::Interact()
{
	if (HasAuthority())
	{
		auto StartPos = GetEyePosition();
		auto EndPos = StartPos + GetForwardVector() * 200;
		auto World = GetWorld();

		FCollisionQueryParams TraceParams;
		//TraceParams.bTraceAsyncScene = true;
		//TraceParams.bReturnPhysicalMaterial = true;
		TraceParams.AddIgnoredActor(this);

		FCollisionObjectQueryParams CollisionParams;
		CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
		CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
		CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
		CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
		CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

		FHitResult OutHit;
		const bool bHit = World->LineTraceSingle(OutHit, StartPos, EndPos, TraceParams, CollisionParams);
		if (bHit)
		{
			// OutHit Contains Hit Data...
			auto Interactable = Cast<AInteractable>(OutHit.Actor.Get());
			if (Interactable)
			{
				if (Interactable->CanInteract(this))
				{
					Interactable->Interact(this);
				}
			}
		}
	}
	else
	{
		SERVER_Interact();
	}
}

bool ATUH_Player::SERVER_Interact_Validate()
{
	return true;
}

void ATUH_Player::SERVER_Interact_Implementation()
{
	Interact();
}

void ATUH_Player::ApplyDamage(float Damage)
{
}

void ATUH_Player::Tick(float DeltaTime)
{
	auto World = GetWorld();

	FCollisionQueryParams TraceParams;
	//TraceParams.bReturnPhysicalMaterial = true;
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams CollisionParams;
	CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	CollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	auto StartPos = GetEyePosition();
	auto EndPos = StartPos + GetForwardVector() * 200;

	if (Interactable)
	{
		Interactable->SetHighlight(false);
	}

	FHitResult OutHit;
	const bool bHit = World->LineTraceSingle(OutHit, StartPos, EndPos, TraceParams, CollisionParams);
	if (bHit)
	{
		Interactable = Cast<AInteractable>(OutHit.Actor.Get());
		if (Interactable)
		{
			Interactable->SetHighlight(true);
		}
	}
}