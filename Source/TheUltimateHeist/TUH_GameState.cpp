#include "TheUltimateHeist.h"
#include "Engine/ActorChannel.h"
#include "UnrealNetwork.h"
#include "TUH_GameState.h"

ATUH_GameState::ATUH_GameState(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATUH_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATUH_GameState, CurrentObjective);
}

void ATUH_GameState::OnRep_CurrentObjective()
{
	UE_LOG(TUHLog, Log, TEXT("Objective changed"));
}

void ATUH_GameState::BeginPlay()
{
	Super::BeginPlay();

	auto World = GetWorld();
	HighlightManager = World->SpawnActor<AHighlightManager>();
}

void ATUH_GameState::Tick(float DeltaTime)
{
	if (HighlightManager && HasAuthority())
	{
		HighlightManager->UpdateHighlights(DeltaTime);
	}
}