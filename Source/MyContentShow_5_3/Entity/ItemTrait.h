// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentTrait.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "MassRepresentationSubsystem.h"
#include "ItemTrait.generated.h"


class UBuildingSubsystem;

USTRUCT()
struct FItemFragment : public FMassFragment
{
	GENERATED_BODY()
	UPROPERTY()
	TEnumAsByte<EResourceType> ItemType;

	UPROPERTY()
	FVector OldLocation;

	UPROPERTY()
	bool bClaimed = false;
};

USTRUCT()
struct FItemAddedToGrid : public FMassTag
{
	GENERATED_BODY()
	
};

UCLASS()
class UItemInitializerProcessor : public UMassObserverProcessor
{
	GENERATED_BODY()
	UItemInitializerProcessor();
public:
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;
	virtual void Initialize(UObject& Owner) override;

	FMassEntityQuery EntityQuery;

	UPROPERTY()
	UBuildingSubsystem* BuildingSubSystem;

	UPROPERTY()
	TObjectPtr<UMassRepresentationSubsystem> RepresentationSubsystem;
};

UCLASS()
class UItemProcessor : public UMassProcessor
{
	GENERATED_BODY()
	UItemProcessor();
public:
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
	virtual void ConfigureQueries() override;
	virtual void Initialize(UObject& Owner) override;

	FMassEntityQuery EntityQuery;

	UPROPERTY()
	UBuildingSubsystem* BuildingSubSystem;

	UPROPERTY()
	UMassRepresentationSubsystem* RepresentationSubsystem;
};

UCLASS()
class MYCONTENTSHOW_5_3_API UItemTrait : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};
