// Copyright Epic Games, Inc. All Rights Reserved.

#include "LinBlueprintLibBPLibraryRt.h"
#include "DataTableEditorUtils.h"

ULinBlueprintLibBPLibraryRt::ULinBlueprintLibBPLibraryRt(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float ULinBlueprintLibBPLibraryRt::LinBlueprintLibSampleFunction(float Param)
{
	return -1;
}


float ULinBlueprintLibBPLibraryRt::GetRuntimeFloatCurveValue(const FRuntimeFloatCurve& CustomCurve, float inTime)
{
	UE_LOG(LogTemp, Display, TEXT("GetRuntimeFloatCurveValue"));
	return CustomCurve.GetRichCurveConst()->Eval(inTime, 0);
}

bool ULinBlueprintLibBPLibraryRt::IsGame(UObject* WorldContextObject)
{
	EWorldType::Type WorldType = EWorldType::None;
	if (WorldContextObject)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
		if(World)
			WorldType = World->WorldType;
	}

	return WorldType == EWorldType::Game || WorldType == EWorldType::GamePreview || WorldType == EWorldType::PIE;
}

bool ULinBlueprintLibBPLibraryRt::GetSoftObjectRef_SkeleMesh(FString AssetPath, TSoftObjectPtr<USkeletalMesh>& OutSoftObjectRef)
{
	OutSoftObjectRef = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(AssetPath));
	return OutSoftObjectRef.IsValid();
}

UObject* ULinBlueprintLibBPLibraryRt::LoadAssetFromPath(FString AssetPath)
{
	return LoadObject<UObject>(nullptr, *AssetPath);
}

bool ULinBlueprintLibBPLibraryRt::Generic_AddRowDT(void* Target, FProperty* Pro, UDataTable* DT, FName NewRowName)
{
	if (!Target || !Pro || !DT)
	{
		return false;
	}
	FStructProperty* StructPro = CastField<FStructProperty>(Pro);
	if (StructPro->Struct != DT->RowStruct)
	{
		return false;
	}
	TMap<FName, uint8*>& DTMap = const_cast<TMap<FName, uint8*>&>(DT->GetRowMap());
	UScriptStruct& EmptyUsingStruct = *DT->RowStruct;
	uint8* NewRawRowData = (uint8*)FMemory::Malloc(EmptyUsingStruct.GetStructureSize());
	EmptyUsingStruct.InitializeStruct(NewRawRowData);
	EmptyUsingStruct.CopyScriptStruct(NewRawRowData, Target);
	DTMap.Add(NewRowName, NewRawRowData);
	DT->Modify();
	FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowData);
	FDataTableEditorUtils::BroadcastPostChange(DT, FDataTableEditorUtils::EDataTableChangeInfo::RowList);
	return true;
}
