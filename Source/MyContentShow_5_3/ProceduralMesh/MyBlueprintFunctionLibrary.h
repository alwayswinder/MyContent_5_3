// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "MyBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MYCONTENTSHOW_5_3_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "VertexPaint")
	static void PaintVerticesLerpAlongAxis(UStaticMeshComponent* StaticMeshComponent, const FLinearColor& StartColor,
		const FLinearColor& EndColor, EVertexPaintAxis Axis, bool bConvertToSRGB = true);
	
	UFUNCTION(BlueprintCallable, Category = "WorldTools")
	static void GetChangeList();
};
