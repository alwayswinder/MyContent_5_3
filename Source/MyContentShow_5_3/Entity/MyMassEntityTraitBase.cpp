// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMassEntityTraitBase.h"
#include "MassCommonFragments.h"
#include "MassEntitySubsystem.h"
#include "MassEntityTemplateRegistry.h"
#include "MassExecutionContext.h"

USimpleRandomMovementProcessor::USimpleRandomMovementProcessor()
	:EntityQuery(*this)
{
	bAutoRegisterWithProcessingPhases = true;
	ExecutionFlags = (int32)EProcessorExecutionFlags::All;
	ExecutionOrder.ExecuteBefore.Add(UE::Mass::ProcessorGroupNames::Avoidance);
}

void USimpleRandomMovementProcessor::ConfigureQueries()
{
	EntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
	EntityQuery.AddRequirement<FSimpleMovementFragment>(EMassFragmentAccess::ReadWrite);
}

void USimpleRandomMovementProcessor::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
	EntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context)
	{
		const TArrayView<FTransformFragment> TransformsList = Context.GetMutableFragmentView<FTransformFragment>();
		const TArrayView<FSimpleMovementFragment> SimpleMovementList = Context.GetMutableFragmentView<FSimpleMovementFragment>();
		const float WorldDeltaTime = Context.GetDeltaTimeSeconds();
	
		for (int32 EntityIndex = 0;EntityIndex < Context.GetNumEntities(); ++EntityIndex)
		{
			FTransform& Transform = TransformsList[EntityIndex].GetMutableTransform();
			FVector& MoveTarget = SimpleMovementList[EntityIndex].Target;
	
			FVector CurrentLocation = Transform.GetLocation();
			FVector TargetVector = MoveTarget - CurrentLocation;
	
			if(TargetVector.Size() <= 20.f)
			{
				MoveTarget = FVector(FMath::RandRange(-1.f, 1.f)*1000.f,
					FMath::RandRange(-1.f, 1.f)*1000.f,CurrentLocation.Z);
			}
			else
			{
				Transform.SetLocation(CurrentLocation + TargetVector.GetSafeNormal()*400.f * WorldDeltaTime);
			}
		}
	});
}

void UMyMassEntityTraitBase::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
	BuildContext.AddFragment<FSimpleMovementFragment>();
}
