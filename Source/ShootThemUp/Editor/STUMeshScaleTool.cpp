// Shoot Them Up. All Rights Reserved.

#include "STUMeshScaleTool.h"

#if WITH_EDITOR
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Editor.h"
#include "Selection.h"
#include "LevelEditor.h"
#include "ToolMenus.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ScopedTransaction.h"
#include "StaticMeshEditorSubsystem.h"
#include "StaticMeshEditorSubsystemHelpers.h"

#define LOCTEXT_NAMESPACE "STUMeshScaleTool"

namespace
{
	void ReduceMeshQualityImpl(float PercentTriangles)
	{
		if (!GEditor || !GEditor->GetSelectedActors()) return;

		UStaticMeshEditorSubsystem* StaticMeshEditor = GEditor->GetEditorSubsystem<UStaticMeshEditorSubsystem>();
		if (!StaticMeshEditor) return;

		TSet<UStaticMesh*> UniqueMeshes;
		for (FSelectionIterator It(GEditor->GetSelectedActorIterator()); It; ++It)
		{
			AActor* Actor = Cast<AActor>(*It);
			if (!Actor) continue;

			UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
			if (!MeshComp || !MeshComp->GetStaticMesh()) continue;

			UStaticMesh* Mesh = MeshComp->GetStaticMesh();
			if (Mesh->IsAsset() && IsValid(Mesh))
			{
				UniqueMeshes.Add(Mesh);
			}
		}

		if (UniqueMeshes.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("STUMeshScaleTool: Select actors with static mesh components."));
			return;
		}

		FScopedTransaction Transaction(LOCTEXT("ReduceMeshQuality", "Reduce Mesh Quality"));

		int32 Processed = 0;
		for (UStaticMesh* StaticMesh : UniqueMeshes)
		{
			StaticMesh->Modify();

			FStaticMeshReductionOptions Options;
			Options.bAutoComputeLODScreenSize = true;
			FStaticMeshReductionSettings Settings;
			Settings.PercentTriangles = FMath::Clamp(PercentTriangles, 0.01f, 1.0f);
			Settings.ScreenSize = 0.5f;
			Options.ReductionSettings.Add(Settings);

			int32 LodCount = StaticMeshEditor->SetLods(StaticMesh, Options);
			if (LodCount >= 2)
			{
				int32 CopyResult = StaticMeshEditor->SetLodFromStaticMesh(StaticMesh, 0, StaticMesh, 1, true);
				if (CopyResult >= 0)
				{
					StaticMeshEditor->RemoveLods(StaticMesh);
					StaticMesh->PostEditChange();
					StaticMesh->MarkPackageDirty();
					++Processed;
				}
			}
		}

		GEditor->RedrawLevelEditingViewports();
		UE_LOG(LogTemp, Log, TEXT("STUMeshScaleTool: reduced quality of %d mesh(es) to %.0f%% triangles."), Processed, PercentTriangles * 100.f);
	}
}

void STUMeshScaleTool::RegisterMenu()
{
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateStatic(&STUMeshScaleTool::RegisterMenusImpl));
}

static void AddMeshQualityEntries(FToolMenuSection& Section)
{
	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		FName("STUReduceMeshQuality50"),
		LOCTEXT("ReduceMeshQuality50", "Reduce Mesh Quality (50% triangles)"),
		LOCTEXT("ReduceMeshQuality50Tooltip", "Simplify selected static meshes to 50% of current polygon count"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() { STUMeshScaleTool::ReduceMeshQuality(0.5f); }))
	));

	Section.AddEntry(FToolMenuEntry::InitMenuEntry(
		FName("STUReduceMeshQuality25"),
		LOCTEXT("ReduceMeshQuality25", "Reduce Mesh Quality (25% triangles)"),
		LOCTEXT("ReduceMeshQuality25Tooltip", "Simplify selected static meshes to 25% of current polygon count"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateLambda([]() { STUMeshScaleTool::ReduceMeshQuality(0.25f); }))
	));
}

void STUMeshScaleTool::RegisterMenusImpl()
{
	FToolMenuOwnerScoped OwnerScoped(FToolMenuOwner("ShootThemUp"));

	if (UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar"))
	{
		FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("STUMeshTools");
		ToolbarSection.Label = LOCTEXT("STUMeshToolsSection", "STU Mesh Tools");
		AddMeshQualityEntries(ToolbarSection);
	}

	if (UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools"))
	{
		FToolMenuSection& ToolsSection = ToolsMenu->FindOrAddSection("STUMeshTools");
		ToolsSection.Label = LOCTEXT("STUMeshToolsSection", "STU Mesh Tools");
		AddMeshQualityEntries(ToolsSection);
	}
}

void STUMeshScaleTool::ReduceMeshQuality(float PercentTriangles)
{
	ReduceMeshQualityImpl(PercentTriangles);
}

#undef LOCTEXT_NAMESPACE
#endif
