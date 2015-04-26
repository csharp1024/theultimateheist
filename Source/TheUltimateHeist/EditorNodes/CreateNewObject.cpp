#include "TheUltimateHeist.h"
#include "CreateNewObject.h"

UObject * ACreateNewObject::NewObjectFromBlueprint(UObject * WorldContextObject, TSubclassOf<UObject> UC)
{
	auto World = GEngine->GetWorldFromContextObject(WorldContextObject);
	auto tempObject = StaticConstructObject(UC);

	return tempObject;
}