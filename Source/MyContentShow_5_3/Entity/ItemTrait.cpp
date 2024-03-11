// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTrait.h"

#include "BuildingSubsystem.h"
#include "MassCommonFragments.h"
#include "MassRepresentationFragments.h"

UItemInitializerProcessor::UItemInitializerProcessor()
	:EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ObservedType = FTransformFragment::StaticStruct();
	Operation = EMassObservedOperation::Add;
}

void UItemInitializerProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TArrayView<FTransformFragment> Transforms = Context.GetMutableFragmentView<FTransformFragment>();
		TArrayView<FItemFragment> ItemFragments = Context.GetMutableFragmentView<FItemFragment>();

		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			FItemFragment& Item = ItemFragments[EntityIndex];
			FTransform& Transfrom = Transforms[EntityIndex].GetMutableTransform();

			if(Transfrom.GetLocation() == FVector::Zero())
			{
				Item.OldLocation.X += FMath::FRandRange(-100.f, 100.f);
				Item.OldLocation.Y += FMath::FRandRange(-100.f, 100.f);
				Transfrom.SetLocation(Item.OldLocation);
			}

			BuildingSubSystem->ItemHashGrid.InsertPoint(Context.GetEntity(EntityIndex), Item.OldLocation);

			Context.Defer().AddTag<FItemAddedToGrid>(Context.GetEntity(EntityIndex));
		}
	});
}

void UItemInitializerProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FItemFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassRepresentationFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FMassRepresentationLODFragment>(EMassFragmentAccess::ReadOnly);
}

void UItemInitializerProcessor::Initialize(UObject& Owner)
{
	UBuildingSubsystem* BuildingSubsystem = Owner.GetWorld()->GetSubsystem<UBuildingSubsystem>();
}

UItemProcessor::UItemProcessor()
	:EntityQuery(*this)
{
}

void UItemProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		TConstArrayView<FTransformFragment> Transforms = Context.GetFragmentView<FTransformFragment>();
		TArrayView<FItemFragment> ItemFragments = Context.GetMutableFragmentView<FItemFragment>();
		
		for (int32 EntityIndex = 0; EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			FItemFragment& Item = ItemFragments[EntityIndex];
			const FVector& Location = Transforms[EntityIndex].GetTransform().GetLocation();

			BuildingSubSystem->ItemHashGrid.UpdatePoint(Context.GetEntity(EntityIndex), Item.OldLocation, Location);
			Item.OldLocation = Location;
		}
	});
}

void UItemProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FItemFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddTagRequirement<FItemAddedToGrid>(EMassFragmentPresence::All);
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadOnly);
}

void UItemProcessor::Initialize(UObject& Owner)
{
	UBuildingSubsystem* BuildingSubsystem = Owner.GetWorld()->GetSubsystem<UBuildingSubsystem>();
}

void UItemTrait::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	Super::BuildTemplate(BuildContext, World);
}
