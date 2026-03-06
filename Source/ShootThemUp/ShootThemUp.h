// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#if WITH_EDITOR
#include "STUMeshScaleTool.h"
#endif

class FShootThemUpGameModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{
		FDefaultGameModuleImpl::StartupModule();
#if WITH_EDITOR
		STUMeshScaleTool::RegisterMenu();
#endif
	}
};

