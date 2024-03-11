// Fill out your copyright notice in the Description page of Project Settings.


#include "AgentTrait.h"

#include "BuildingSubsystem.h"
#include "MassEntityManager.h"
#include "MassEntityTemplateRegistry.h"
#include "MassRepresentationFragments.h"

UAgentInitializer::UAgentInitializer()
	:EntityQuery(*this)
{
	ObservedType = FAgentFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void UAgentInitializer::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		UBuildingSubsystem* BuildingSubsystem = GetWorld()->GetSubsystem<UBuildingSubsystem>();
		
		for (int32 EntityIndex = 0;EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			BuildingSubsystem->AddAgent(Context.GetEntity(EntityIndex));
		}

	});

}

void UAgentInitializer::ConfigureQueries()
{
	EntityQuery.AddRequirement<FAgentFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddConstSharedRequirement<FAgentParameters>(EMassFragmentPresence::All);
	EntityQuery.AddTagRequirement<FAgent>(EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadWrite);
}

void UAgentTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(World);
	
	BuildContext.AddFragment<FAgentFragment>();
	BuildContext.AddTag<FAgent>();
	
	const FConstSharedStruct AgentFragment = EntityManager.GetOrCreateConstSharedFragment(AgentParamters);
}
