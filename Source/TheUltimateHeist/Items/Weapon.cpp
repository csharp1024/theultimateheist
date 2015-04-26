// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "UnrealNetwork.h"
#include "Interfaces/Armed.h"
#include "Weapon.h"

// Sets default values
AWeapon::AWeapon(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AttachParent = RootComponent;
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(AWeapon, MyPawn);
}

void AWeapon::OnRep_MyPawn()
{
	UE_LOG(TUHLog, Log, TEXT("[OnRep_MyPawn] %s: %d"), LOG_NETMODE(), MyPawn.GetObject());
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		//OnLeaveInventory();
	}
}

void AWeapon::SetOwningPawn(const TScriptInterface<IArmed> & NewOwner)
{
	UE_LOG(TUHLog, Log, TEXT("[SetOwningPawn] %s:"), LOG_NETMODE());
	if (MyPawn != NewOwner)
	{
		auto Pawn = Cast<APawn>(NewOwner.GetObject());
		Instigator = Pawn;
		MyPawn = NewOwner;
		// net owner for RPC calls
		SetOwner(Pawn);
	}
}

void AWeapon::OnEnterInventory(const TScriptInterface<IArmed> & Pawn)
{
	UE_LOG(TUHLog, Log, TEXT("[OnEnterInventory] %s:"), LOG_NETMODE());
	SetOwningPawn(Pawn);
}

void AWeapon::OnEquip()
{
	AttachMeshToPawn();
}

void AWeapon::OnUnEquip()
{
	DetachMeshFromPawn();
}

void AWeapon::AttachMeshToPawn()
{
	if (MyPawn)
	{
		DetachMeshFromPawn();

		auto Pawn = Cast<APawn>(MyPawn.GetObject());
		auto PawnMesh = MyPawn->GetSpecificPawnMesh(Pawn->IsLocallyControlled());
		Mesh->SetHiddenInGame(false);
		Mesh->AttachTo(PawnMesh, MyPawn->GetWeaponAttachPoint(Pawn->IsLocallyControlled()));
	}
}

void AWeapon::DetachMeshFromPawn()
{
	Mesh->DetachFromParent();
	Mesh->SetHiddenInGame(true);
}

void AWeapon::Shoot()
{
	if (HasAuthority())
	{
		if (SERVER_Shoot_Validate())
		{
			SERVER_Shoot_Implementation();
		}
	}
	else
	{
		SERVER_Shoot();
	}
}

bool AWeapon::SERVER_Shoot_Validate()
{
	return !Shooting;
}

void AWeapon::SERVER_Shoot_Implementation()
{
	Shooting = true;
	MULTICAST_Shoot();

	OnShoot.Broadcast();

	Time = 0;
	auto EyePosition = MyPawn->GetEyePosition();
	auto Forward = MyPawn->GetForwardVector();

	FHitResult HitResult;
	FCollisionQueryParams Params;
	if (ActorLineTraceSingle(HitResult, EyePosition, EyePosition + Forward * 10000, ECollisionChannel::ECC_Pawn, Params))
	{

	}
}

void AWeapon::MULTICAST_Shoot_Implementation()
{
	// spawn emitter
	// play sound
	// play animation
}

void AWeapon::SetMesh(USkeletalMesh * SkeletalMesh)
{
	Mesh->SetSkeletalMesh(SkeletalMesh);
}
