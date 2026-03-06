// Shoot Them Up. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
class STUMeshScaleTool
{
public:
	static void RegisterMenu();
	/** Reduce polygon count of selected static mesh assets (from selected actors). */
	static void ReduceMeshQuality(float PercentTriangles);

private:
	static void RegisterMenusImpl();
};
#endif
