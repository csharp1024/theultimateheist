#pragma once

#include "GameFramework/GameState.h"
#include "GameManagers/HighlightManager.h"
#include "TUH_GameState.generated.h"

UCLASS()
class ATUH_GameState : public AGameState
{
	GENERATED_BODY()

public:
	ATUH_GameState(const FObjectInitializer & ObjectInitializer);

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, Category = "GameState")
		AHighlightManager * HighlightManager;

	virtual void Tick(float DeltaTime) override;
};