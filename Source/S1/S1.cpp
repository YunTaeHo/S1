// Copyright Epic Games, Inc. All Rights Reserved.

#include "S1.h"
#include "Modules/ModuleManager.h"
#include "System/S1AssetMaanger.h"

/*
 *	FS1GameMoudle
 */
class FS1GameModule : public FDefaultGameModuleImpl
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FS1GameModule::StartupModule()
{
	US1AssetMaanger::TestClone();
}

void FS1GameModule::ShutdownModule()
{
}


IMPLEMENT_PRIMARY_GAME_MODULE( FS1GameModule , S1, "S1" );

