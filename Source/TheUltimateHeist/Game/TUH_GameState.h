#pragma once

#include "GameFramework/GameState.h"
#include "GameManagers/HighlightManager.h"
#include "TUH_GameState.generated.h"

class UObjective;

UCLASS()
class ATUH_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ATUH_GameState(const FObjectInitializer & ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "GameState")
		AHighlightManager * HighlightManager;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentObjective, Category = "GameState | Objective")
		UObjective * CurrentObjective;

	UFUNCTION()
		void OnRep_CurrentObjective();

	void SetCurrentObjective(UObjective * Objective);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};