// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentTrait.h"
#include "MassEntityConfigAsset.h"
#include "MassSmartObjectBehaviorDefinition.h"
#include "GatherBD.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTSHOW_5_3_API UGatherBD : public USmartObjectMassBehaviorDefinition
{
	GENERATED_BODY()
public:
	virtual void Activate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const override;
	virtual  void Deactivate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const override;

	UPROPERTY(EditDefaultsOnly, Category = SmartObject)
	int ResourceAmout = 1;

	UPROPERTY(EditDefaultsOnly, Category = SmartObject)
	TEnumAsByte<EResourceType> ResourceType = Tree;

	UPROPERTY(EditDefaultsOnly, Category = SmartObject)
	UMassEntityConfigAsset* ItemConfig;
};
