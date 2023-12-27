// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Curves/CurveVector.h"
#include "MyProceduralMeshActor.generated.h"

UCLASS()
class  AMyProceduralMeshActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyProceduralMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	FVector2D GridSize = FVector2D(1000, 1000);
	// X 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel_X = 10;
	// Y 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel_Y = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	bool Enable_Mountain = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	int32 NoiseSeed_Mountain = 7777;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseHeight_Mountain = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseFrequency_Mountain = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FRuntimeVectorCurve NoiseDistribution_Mountain;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	bool Enable_Lake= false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	int32 NoiseSeed_Lake = 7777;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseHeight_Lake = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseFrequency_Lake = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FRuntimeVectorCurve NoiseDistribution_Lake;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void GenerateMesh();
	UFUNCTION()
	void GenerateGrid(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV0, TArray<FColor>& InVertexColor);
private:
	TMap<FVector2d, FVector>  GetPlanePosFromXY();

};