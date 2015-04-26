#pragma once

#include "GameFramework/Actor.h"
#include "CreateNewObject.generated.h"

UCLASS()
class ACreateNewObject : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", FriendlyName = "Create Object From Blueprint", CompactNodeTitle = "Create", Keywords = "new create blueprint"), Category = Game)
		static UObject * NewObjectFromBlueprint(UObject*WorldContextObject, TSubclassOf<UObject> UC);
};