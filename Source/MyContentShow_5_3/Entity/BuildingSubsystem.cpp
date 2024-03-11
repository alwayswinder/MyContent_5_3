// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSubsystem.h"

#include "ItemTrait.h"
#include "MassEntitySubsystem.h"

void UBuildingSubsystem::AddResourceQueue(FSmartObjectHandle& SOHandle)
{
	if(QueuedResources.Find(SOHandle) == INDEX_NONE)
	{
		QueuedResources.Emplace(SOHandle);
	}
}

void UBuildingSubsystem::AddAgent(const FMassEntityHandle& Entity)
{
	Agents.Emplace(Entity);
}

void UBuildingSubsystem::AddBuilding(const FSmartObjectHandle& BuildingRequest, int Floors)
{
	QueuedBuildings.Emplace(FBuild(BuildingRequest, Floors));
}

bool UBuildingSubsystem::ClaimFloor(FSmartObjectHandle& OutBuilding)
{
	bool bSuccess = false;
	if(QueuedBuildings.Num() > 0)
	{
		FBuild& BuildStruct = QueuedBuildings[0];
		OutBuilding = BuildStruct.BuildingRequest;
		BuildStruct.FloorsNeeded--;

		if(BuildStruct.FloorsNeeded <= 0)
		{
			QueuedBuildings.RemoveAt(0);
			bSuccess = true;
		}
	}
	return bSuccess;
}

bool UBuildingSubsystem::FindItem(const FVector& Location, float Radius, EResourceType ResourceType,
	FMassEntityHandle& OutItemHandle) const
{
	FMassEntityManager& EntityManager = UE::Mass::Utils::GetEntityManagerChecked(*GetWorld());
	
	const TPair<FMassEntityHandle, float> ItemHandle = ItemHashGrid.FindNearestInRadius(Location, Radius,
		[this, &Location, &EntityManager](const FMassEntityHandle& Handle)
		{
			if(!EntityManager.IsEntityValid(Handle))
			{
				return 9999.0;
			}
			FVector& OtherLocation = EntityManager.GetFragmentDataPtr<FItemFragment>(Handle)->OldLocation;
			return FVector::Distance(OtherLocation, Location);
		},[this, &ResourceType, &EntityManager](const FMassEntityHandle& Handle)
		{
			if(!EntityManager.IsEntityValid(Handle))
			{
				return true;
			}
			FItemFragment& Item = EntityManager.GetFragmentDataChecked<FItemFragment>(Handle);
			return Item.bClaimed || Item.ItemType != ResourceType;
		});
	OutItemHandle = ItemHandle.Key;
	return EntityManager.IsEntityValid(ItemHandle.Key);
}

bool UBuildingSubsystem::ClaimResource(FSmartObjectHandle& OutResourceHandle)
{
	bool bSuccess = false;
	if(QueuedResources.Num() > 0)
	{
		OutResourceHandle = QueuedResources[0];
		QueuedResources.RemoveAt(0);
		bSuccess = true;
	}
	return  bSuccess;
}
