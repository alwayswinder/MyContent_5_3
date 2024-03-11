// Fill out your copyright notice in the Description page of Project Settings.


#include "GatherBD.h"

#include "ItemTrait.h"
#include "MassCommonFragments.h"
#include "MassEntitySettings.h"
#include "MassEntitySubsystem.h"
#include "MassSmartObjectFragments.h"
#include "MassSpawnerSubsystem.h"
#include "SmartObjectComponent.h"
#include "SmartObjectSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UGatherBD::Activate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Activate(CommandBuffer, EntityContext);
	
	FAgentFragment& Agent = EntityContext.EntityView.GetFragmentData<FAgentFragment>();
	
	Agent.ResourceHandle.Invalidate();
}

void UGatherBD::Deactivate(FMassCommandBuffer& CommandBuffer, const FMassBehaviorEntityContext& EntityContext) const
{
	Super::Deactivate(CommandBuffer, EntityContext);

	UMassSpawnerSubsystem* SpawnerSubsystem = GetWorld()->GetSubsystem<UMassSpawnerSubsystem>();

	if(EntityContext.SmartObjectSubsystem.GetWorld() && UGameplayStatics::GetPlayerPawn(EntityContext.SmartObjectSubsystem.GetWorld(), 0))
	{
		TArray<FMassEntityHandle> Items;
		const FMassEntityTemplate EntityTemplate = ItemConfig->GetConfig().GetOrCreateEntityTemplate(*EntityContext.SmartObjectSubsystem.GetWorld());
		SpawnerSubsystem->SpawnEntities(EntityTemplate, 4, Items);

		for(const FMassEntityHandle& ItemHandle : Items)
		{
			const FVector& SpawnLocation = EntityContext.EntityView.GetFragmentDataPtr<FTransformFragment>()->GetTransform().GetLocation();

			FItemFragment ItemFragment;
			ItemFragment.ItemType = ResourceType;
			ItemFragment.OldLocation = SpawnLocation;
			CommandBuffer.PushCommand<FMassCommandAddFragmentInstances>(ItemHandle, ItemFragment);
			
		}

		const FMassSmartObjectUserFragment& SOUser = EntityContext.EntityView.GetFragmentData<FMassSmartObjectUserFragment>();
		if(USmartObjectComponent* SOComp = EntityContext.SmartObjectSubsystem.GetSmartObjectComponent(SOUser.InteractionHandle))
		{
			CommandBuffer.PushCommand<FMassDeferredDestroyCommand>([SOComp, EntityContext](FMassEntityManager& System)
			{
				EntityContext.SmartObjectSubsystem.UnregisterSmartObject(*SOComp);
				SOComp->GetOwner()->Destroy();
			});
		}
	}
}
