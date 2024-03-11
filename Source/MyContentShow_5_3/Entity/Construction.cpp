// Fill out your copyright notice in the Description page of Project Settings.


#include "Construction.h"

#include "AgentTrait.h"
#include "MassExecutionContext.h"
#include "MassSmartObjectFragments.h"
#include "SmartObjectComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

UConstruction::UConstruction()
{
	ObservedType = FConstructionFloor::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void UConstruction::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	//添加楼层的地方
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TArrayView<FAgentFragment> Agents = Context.GetMutableFragmentView<FAgentFragment>();
		TConstArrayView<FMassSmartObjectUserFragment> SOUsers = Context.GetFragmentView<FMassSmartObjectUserFragment>();
		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			FAgentFragment& Agent = Agents[EntityIndex];
			const FMassSmartObjectUserFragment& SOUser = SOUsers[EntityIndex];

			if(const USmartObjectComponent* SmartObjectcomponent = SmartObjectSubsystem->GetSmartObjectComponent(SOUser.InteractionHandle))
			{
				const AActor* actor = SmartObjectcomponent->GetOwner();
				UInstancedStaticMeshComponent* InstancedStaticMeshComp = actor->FindComponentByClass<UInstancedStaticMeshComponent>();
				FTransform Transfrom;
				Transfrom.SetLocation(FVector(0,0,IncrementHeight*InstancedStaticMeshComp->GetInstanceCount()));
				InstancedStaticMeshComp->AddInstance(Transfrom);

				Agent.BuildingHandle = FSmartObjectHandle::Invalid;
				Context.Defer().RemoveTag<FConstructionFloor>(Context.GetEntity(EntityIndex));

				int* ResourceAmount = Agent.Inventory.Find(EResourceType::Rock);
				if(ResourceAmount)
				{
					*ResourceAmount -= 1;
				}
				ResourceAmount = Agent.Inventory.Find(EResourceType::Tree);
				if(ResourceAmount)
				{
					*ResourceAmount -= 1;
				}
			}
		}
	});

}

void UConstruction::ConfigureQueries()
{
	EntityQuery.AddRequirement<FAgentFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassSmartObjectUserFragment>(EMassFragmentAccess::ReadOnly);
}

void UConstruction::Initialize(UObject& Owner)
{
	SmartObjectSubsystem = Owner.GetWorld()->GetSubsystem<USmartObjectSubsystem>();
}
