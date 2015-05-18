// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __THEULTIMATEHEIST_H__
#define __THEULTIMATEHEIST_H__

#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(TUHLog, Log, All);

const FString EnumToString(const TCHAR * Enum, int32 EnumValue);

#define LOG_NETMODE() (GetNetMode() < NM_Client ? TEXT("Server") : TEXT("Client"))
#define LOG_NAME(o) (o ? *o->GetName() : TEXT("None"))

#endif
