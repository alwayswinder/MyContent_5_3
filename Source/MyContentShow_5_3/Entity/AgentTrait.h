// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "MassObserverProcessor.h"
#include "SmartObjectTypes.h"
#include "AgentTrait.generated.h"

class UBuildingSubsystem;

UENUM()
enum EResourceType
{
	Tree,
	Rock
};

USTRUCT(BlueprintType)
struct FAgentFragment : public FMassFragment
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Agent")
	TMap<TEnumAsByte<EResourceType>, int> Inventory;

	UPROPERTY()
	FSmartObjectHandle BuildingHandle;

	UPROPERTY()
	FSmartObjectHandle ResourceHandle;

	UPROPERTY()
	TArray<FMassEntityHandle> QueueItems;
};

USTRUCT()
struct FAgentParameters : public  FMassSharedFragment
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EResourceType>, int> DefaultRequiredResources;
};

USTRUCT()
struct FAgent : public  FMassTag
{
	GENERATED_BODY()
	
};

UCLASS()
class UAgentInitializer : public UMassObserverProcessor
{
	GENERATED_BODY()

	UAgentInitializer();

	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;

	TObjectPtr<UBuildingSubsystem> MovementSubsystem;
	TObjectPtr<USmartObjectSubsystem> SmartObjectSubsystem;

	FMassEntityQuery EntityQuery;
};

UCLASS()
class MYCONTENTSHOW_5_3_API UAgentTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()

	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;

	UPROPERTY(EditAnywhere)
	FAgentParameters AgentParamters;
};
