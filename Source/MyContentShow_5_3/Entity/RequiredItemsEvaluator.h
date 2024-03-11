// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentTrait.h"
#include "MassCommonFragments.h"
#include "MassEntitySubsystem.h"
#include "MassStateTreeTypes.h"
#include "SmartObjectSubsystem.h"
#include "StateTreeInstanceData.h"
#include "RequiredItemsEvaluator.generated.h"

struct FMassMoveTargetFragment;
struct FTransfromFragment;
struct FMassSmartObjectUserFragment;
struct FMassMovementParameters;
class UMassSignalSubsystem;

USTRUCT()
struct FequiredItemsEvaluatorData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = Output)
	FSmartObjectRequestFilter Filter;

	UPROPERTY(EditAnywhere, Category = Output)
	FSmartObjectHandle SmartObjectHandle;

	UPROPERTY(EditAnywhere, Category = Output)
	FMassEntityHandle ItemHandle;

	UPROPERTY(EditAnywhere, Category = Output)
	bool bFoundSmartObject = false;

	UPROPERTY(EditAnywhere, Category = Output)
	bool bFoundItemHandle = false;
};
USTRUCT()
struct FRequiredItemsEvaluator : public FMassStateTreeEvaluatorBase
{
	GENERATED_BODY()
	
protected:
	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FequiredItemsEvaluatorData::StaticStruct(); }
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	TStateTreeExternalDataHandle<FAgentFragment> AgentHandle;
	TStateTreeExternalDataHandle<USmartObjectSubsystem> SmartObjectSubsystemHandle;
	TStateTreeExternalDataHandle<UMassEntitySubsystem> EntitySubsystemHandle;
	TStateTreeExternalDataHandle<FTransformFragment> TransformHandle;
	TStateTreeExternalDataHandle<UBuildingSubsystem> BuildingSubsystemHandle;

	
};
