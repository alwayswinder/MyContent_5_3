// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassEntityTraitBase.h"
#include "MassEntityTypes.h"
#include "MassProcessor.h"
#include "MyMassEntityTraitBase.generated.h"

/**
 * 
 */
USTRUCT()
struct FSimpleMovementFragment : public FMassFragment
{
	GENERATED_BODY()
	FVector Target;	
};

UCLASS()
class USimpleRandomMovementProcessor : public UMassProcessor
{
	GENERATED_BODY()

public:
	USimpleRandomMovementProcessor();
protected:
	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;
private:
	FMassEntityQuery EntityQuery;
};

UCLASS()
class MYCONTENTSHOW_5_3_API UMyMassEntityTraitBase : public UMassEntityTraitBase
{
	GENERATED_BODY()
	
protected:
	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override; 
};
