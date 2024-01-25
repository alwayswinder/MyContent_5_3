// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#include "StaticMeshComponentLODInfo.h"
#include "MeshVertexPainter/MeshVertexPainter.h"


void UMyBlueprintFunctionLibrary::PaintVerticesLerpAlongAxis(UStaticMeshComponent* StaticMeshComponent,
                                                             const FLinearColor& StartColor, const FLinearColor& EndColor, EVertexPaintAxis Axis, bool bConvertToSRGB)
{
	if (!StaticMeshComponent || !StaticMeshComponent->GetStaticMesh())
	{
		return;
	}

	const FBoxSphereBounds Bounds = StaticMeshComponent->GetStaticMesh()->GetBounds();
	const FBox Box = Bounds.GetBox();
	static_assert(static_cast<int32>(EVertexPaintAxis::X) == 0, "EVertexPaintAxis not correctly defined");
	const float AxisMin = Box.Min.Component(static_cast<int32>(Axis));
	const float AxisMax = Box.Max.Component(static_cast<int32>(Axis));

	const int32 NumMeshLODs = StaticMeshComponent->GetStaticMesh()->GetNumLODs();
	StaticMeshComponent->SetLODDataCount(NumMeshLODs, NumMeshLODs);

	uint32 LODIndex = 0;
	for (FStaticMeshComponentLODInfo& LODInfo : StaticMeshComponent->LODData)
	{
		StaticMeshComponent->RemoveInstanceVertexColorsFromLOD(LODIndex);
		check(LODInfo.OverrideVertexColors == nullptr);

		const FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[LODIndex];
		const FPositionVertexBuffer& PositionVertexBuffer = LODModel.VertexBuffers.PositionVertexBuffer;
		const uint32 NumVertices = PositionVertexBuffer.GetNumVertices();

		TArray<FColor> VertexColors;
		VertexColors.AddZeroed(NumVertices);

		for (uint32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
		{
			const FVector3f& VertexPosition = PositionVertexBuffer.VertexPosition(VertexIndex);
			const FLinearColor Color = FMath::Lerp(StartColor, EndColor, (VertexPosition.Component(static_cast<int32>(Axis)) - AxisMin) / (AxisMax - AxisMin));
			VertexColors[VertexIndex] = Color.ToFColor(bConvertToSRGB);
		}

		LODInfo.OverrideVertexColors = new FColorVertexBuffer;
		LODInfo.OverrideVertexColors->InitFromColorArray(VertexColors);

		BeginInitResource(LODInfo.OverrideVertexColors);

		LODIndex++;
	}

#if WITH_EDITORONLY_DATA
	StaticMeshComponent->CachePaintedDataIfNecessary();
#endif
	StaticMeshComponent->MarkRenderStateDirty();
	StaticMeshComponent->bDisallowMeshPaintPerInstance = true;
}
