// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AgentTrait.h"
#include "MassEntityTypes.h"
#include "SmartObjectTypes.h"
#include "Spatial/PointHashGrid3.h"
#include "Subsystems/WorldSubsystem.h"
#include "BuildingSubsystem.generated.h"

typedef UE::Geometry::TPointHashGrid3<FMassEntityHandle, Chaos::FReal> ItemHashGrid3D;

USTRUCT()
struct FBuild
{
	GENERATED_BODY()
	UPROPERTY()
	FSmartObjectHandle BuildingRequest;

	UPROPERTY()
	int FloorsNeeded = 1;

	FBuild(){}
	FBuild(const FSmartObjectHandle& BuildingRequest, int FloorsNeeded)
	{
		this->BuildingRequest = BuildingRequest;
		this->FloorsNeeded = FloorsNeeded;
	}

	bool operator==(const FBuild& OtherBuilding) const
	{
		return OtherBuilding.BuildingRequest == this->BuildingRequest;
	}
};
UCLASS()
class MYCONTENTSHOW_5_3_API UBuildingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void AddResourceQueue(FSmartObjectHandle& SOHandle);

	UFUNCTION()
	void AddAgent(const FMassEntityHandle& Entity);

	UFUNCTION(BlueprintCallable)
	void AddBuilding(const FSmartObjectHandle& BuildingRequest,int Floors = 1);

	UFUNCTION()
	bool ClaimFloor(FSmartObjectHandle& OutBuilding);

	bool FindItem(const FVector& Location, float Radius, EResourceType ResourceType, FMassEntityHandle& OutItemHandle)const;

	int GetQueuedBuildings()const{return QueuedBuildings.Num();}

	void GetQueueResources(TArray<FSmartObjectHandle>& OutQueuedResources)const {OutQueuedResources = QueuedResources;}

	bool ClaimResource(FSmartObjectHandle& OutResourceHandle);

	UPROPERTY()
	FMassEntityHandle Agent;

	TArray<FBuild> QueuedBuildings;

	TArray<FMassEntityHandle> Agents;

	TArray<FSmartObjectHandle> QueuedResources;

	ItemHashGrid3D ItemHashGrid = ItemHashGrid3D(500.0f, FMassEntityHandle());
};
