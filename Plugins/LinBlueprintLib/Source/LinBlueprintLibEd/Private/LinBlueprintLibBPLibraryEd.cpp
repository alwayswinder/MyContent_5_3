// Copyright Epic Games, Inc. All Rights Reserved.

#include "LinBlueprintLibBPLibraryEd.h"

#include "ContentBrowserModule.h"
//#include "DataTableEditorUtils.h"
#include "IContentBrowserSingleton.h"
#include "IMeshMergeUtilities.h"
#include "LevelEditorViewport.h"
#include "MeshMergeModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Components/SplineMeshComponent.h"

ULinBlueprintLibBPLibraryEd::ULinBlueprintLibBPLibraryEd(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float ULinBlueprintLibBPLibraryEd::LinBlueprintLibSampleFunction(float Param)
{
	return -1;
}


// bool ULinBlueprintLibBPLibraryEd::Generic_AddRowDT(void* Target, FProperty* Pro, UDataTable* DT, FName NewRowName)
// {
//  	if (!Target || !Pro || !DT)
//  	{
//  		return false;
//  	}
//  	FStructProperty* StructPro = CastField<FStructProperty>(Pro);
//  	if (StructPro->Struct != DT->RowStruct)
//  	{
//  		return false;
//  	}
//  	TMap<FName, uint8*>& DTMap = const_cast<TMap<FName, uint8*>&>(DT->GetRowMap());
//  	UScriptStruct& EmptyUsingStruct = *DT->RowStruct;
//  	uint8* NewRawRowData = (uint8*)FMemory::Malloc(EmptyUsingStruct.GetStructureSize());
//  	EmptyUsingStruct.InitializeStruct(NewRawRowData);
//  	EmptyUsingStruct.CopyScriptStruct(NewRawRowData, Target);
//  	DTMap.Add(NewRowName, NewRawRowData);
//  	DT->Modify();
//  	FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowData);
//  	FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowList);
//  	return true;
// }


void CreateStaticAsset(const TArray<UPrimitiveComponent*> &PrimitiveComponentsToMerge, const FString &PackageName, UWorld* World)
{
	const IMeshMergeUtilities& MeshMergeUtilities = FModuleManager::Get().LoadModuleChecked<IMeshMergeModule>("MeshMergeUtilities").GetUtilities();
	
	FMeshMergingSettings MergeSettings;
	MergeSettings.bMergeMaterials = false;
	MergeSettings.LODSelectionType = EMeshLODSelectionType::AllLODs;
	
	TArray<UObject*> AssetsToSync;
	constexpr float ScreenAreaSize = TNumericLimits<float>::Max();
	FVector MMWL;
	MeshMergeUtilities.MergeComponentsToStaticMesh( PrimitiveComponentsToMerge, World, MergeSettings,
		nullptr, nullptr, PackageName, AssetsToSync, MMWL, ScreenAreaSize, true);

	if (AssetsToSync.Num())
	{
		FAssetRegistryModule& AssetRegistry = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		int32 AssetCount = AssetsToSync.Num();
		for (int32 AssetIndex = 0; AssetIndex < AssetCount; AssetIndex++)
		{
			AssetRegistry.AssetCreated(AssetsToSync[AssetIndex]);
			GEditor->BroadcastObjectReimported(AssetsToSync[AssetIndex]);
		}

		//Also notify the content browser that the new assets exists
		FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		ContentBrowserModule.Get().SyncBrowserToAssets(AssetsToSync, true);
	}
}
bool ULinBlueprintLibBPLibraryEd::MergeCompontentsToMesh(const AActor* ActorSelected, const FString &PackageName, int EachNum)
{
	int AssetIndex = 1;
	FString AssetName = PackageName;
	TArray<UActorComponent*> AllSplineMeshComponent = ActorSelected->K2_GetComponentsByClass(USplineMeshComponent::StaticClass());
	TArray<UPrimitiveComponent*> PrimitiveComponentsToMerge; 
	for (auto SplineMeshComponent : AllSplineMeshComponent)
	{
		PrimitiveComponentsToMerge.Add(Cast<UPrimitiveComponent>(SplineMeshComponent));
		if (PrimitiveComponentsToMerge.Num() >= EachNum)
		{
			CreateStaticAsset(PrimitiveComponentsToMerge, AssetName + FString::FromInt(AssetIndex), ActorSelected->GetWorld());
			PrimitiveComponentsToMerge.Empty();
			AssetIndex++;
		}
	}
	CreateStaticAsset(PrimitiveComponentsToMerge, AssetName + FString::FromInt(AssetIndex), ActorSelected->GetWorld());
	
	return true;
}

FVector ULinBlueprintLibBPLibraryEd::GetCoursorPosition()
{
	FViewportCursorLocation CursorLocation = GCurrentLevelEditingViewportClient->GetCursorWorldLocationFromMousePos();
	FVector LineCheckStart = CursorLocation.GetOrigin();
	FVector LineCheckEnd = CursorLocation.GetOrigin() + CursorLocation.GetDirection() * 10000;

	// Perform a line check from the camera eye to the surface to place the preview mesh. 
	FHitResult Hit(ForceInit);
	FCollisionQueryParams LineParams(SCENE_QUERY_STAT(ULinBlueprintLibBPLibraryEd), true);
	LineParams.bTraceComplex = true;
	if ( GWorld->LineTraceSingleByChannel(Hit, LineCheckStart, LineCheckEnd, ECC_Visibility, LineParams) ) 
	{
		return Hit.Location;
	}
	
	return FVector();
 }
