// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TheUltimateHeist.h"
#include "TheUltimateHeistCharacter.h"
#include "Animation/AnimInstance.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/InputSettings.h"
#include "AI/TUH_AIController.h"
#include "UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ATheUltimateHeistCharacter

ATheUltimateHeistCharacter::ATheUltimateHeistCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	Mesh1P->AttachParent = FirstPersonCameraComponent;
	Mesh1P->RelativeLocation = FVector(0.f, 0.f, -150.f);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	//GetMesh()->bRenderCustomDepth = 1;
	GetMesh()->bRenderCustomTag = 1;
	GetMesh()->CustomTagColor = FLinearColor(1.0F, 0.0F, 1.0F);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATheUltimateHeistCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATheUltimateHeistCharacter, Health);
}

void ATheUltimateHeistCharacter::Tick(float DeltaTime)
{
	if (HasAuthority())
	{
		if (Health <= 0)
		{
			Die();
		}
	}
}

void ATheUltimateHeistCharacter::ApplyDamage(float Damage)
{
	if (HasAuthority())
	{
		Health -= Damage;
	}
}

void ATheUltimateHeistCharacter::Die()
{
	if (HasAuthority() && !Dead)
	{
		Dead = true;

		MULTICAST_Die();

		auto AIController = Cast<ATUH_AIController>(Controller);
		if (AIController)
		{
			AIController->Killed();
		}
	}
}

void ATheUltimateHeistCharacter::MULTICAST_Die_Implementation()
{
	GetMesh()->PlayAnimation(DeathAnim, false);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATheUltimateHeistCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATheUltimateHeistCharacter::TouchStarted);
	if (EnableTouchscreenMovement(InputComponent) == false)
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &ATheUltimateHeistCharacter::OnFire);
	}

	InputComponent->BindAxis("MoveForward", this, &ATheUltimateHeistCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATheUltimateHeistCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATheUltimateHeistCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATheUltimateHeistCharacter::LookUpAtRate);
}

void ATheUltimateHeistCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ATheUltimateHeistCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

void ATheUltimateHeistCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ATheUltimateHeistCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATheUltimateHeistCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATheUltimateHeistCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATheUltimateHeistCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ATheUltimateHeistCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATheUltimateHeistCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &ATheUltimateHeistCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATheUltimateHeistCharacter::TouchUpdate);
	}
	return bResult;
}

void ATheUltimateHeistCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority())
	{
		auto PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
		PerceptionSystem->RegisterSource<UAISense_Sight>(*this);
	}
}

float ATheUltimateHeistCharacter::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	auto PlayerMesh = IsLocallyControlled() ? GetMesh1P() : GetMesh();
	UAnimInstance * AnimInstance = (PlayerMesh) ? PlayerMesh->GetAnimInstance() : NULL;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			// Start at a given Section.
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName);
			}

			return Duration;
		}
	}

	return 0.f;
}
