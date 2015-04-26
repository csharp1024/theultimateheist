// Fill out your copyright notice in the Description page of Project Settings.

#include "TheUltimateHeist.h"
#include "UnrealNetwork.h"
#include "Interfaces/Armed.h"
#include "Gun.h"

// Sets default values
AGun::AGun(const FObjectInitializer & ObjectInitializer)
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

void AGun::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	DOREPLIFETIME(AGun, Armed);
}

void AGun::OnRep_MyPawn()
{
	UE_LOG(TUHLog, Log, TEXT("[OnRep_MyPawn] %s: %d"), LOG_NETMODE(), MyPawn.GetObject());
	SetOwningPawn(MyPawn);
}

void AGun::SetOwningPawn(const TScriptInterface<IArmed> & NewOwner)
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

void AGun::OnEnterInventory(const TScriptInterface<IArmed> & Pawn)
{
	UE_LOG(TUHLog, Log, TEXT("[OnEnterInventory] %s:"), LOG_NETMODE());
	SetOwningPawn(Pawn);
}

void AGun::Shoot()
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

bool AGun::SERVER_Shoot_Validate()
{
	return Armed && !Shooting;
}

void AGun::SERVER_Shoot_Implementation()
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

void AGun::MULTICAST_Shoot_Implementation()
{
	// spawn emitter
	// play sound
	// play animation
}

void AGun::DrawGun()
{
	UE_LOG(TUHLog, Log, TEXT("[DrawGun] %s: "), LOG_NETMODE());
	if (HasAuthority())
	{
		if (SERVER_DrawGun_Validate())
		{
			SERVER_DrawGun_Implementation();
		}
	}
	else
	{
		SERVER_DrawGun();
	}
}

bool AGun::SERVER_DrawGun_Validate()
{
	return !Armed;
}

void AGun::SERVER_DrawGun_Implementation()
{
	if (HasAuthority() && !Armed)
	{
		Armed = true;
		OnRep_Armed();
	}
}

void AGun::OnRep_Armed()
{
	UE_LOG(TUHLog, Log, TEXT("[OnRepArmed] %s: %d"), LOG_NETMODE(), MyPawn.GetObject());
	if (MyPawn)
	{
		MyPawn->PositionGun();
	}
}

void AGun::SetMesh(USkeletalMesh * SkeletalMesh)
{
	Mesh->SetSkeletalMesh(SkeletalMesh);
}
