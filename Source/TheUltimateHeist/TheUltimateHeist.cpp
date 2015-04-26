// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TheUltimateHeist.h"


IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, TheUltimateHeist, "TheUltimateHeist" );
DEFINE_LOG_CATEGORY(TUHLog);
 
const FString EnumToString(const TCHAR * Enum, int32 EnumValue)
{
	auto EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
	if (!EnumPtr)
	{
		return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();
	}

#if WITH_EDITOR
	return EnumPtr->GetDisplayNameText(EnumValue).ToString();
#else
	return EnumPtr->GetEnumName(EnumValue);
#endif
}