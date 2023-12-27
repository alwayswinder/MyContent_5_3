// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Curves/CurveVector.h"
#include "MyProceduralSphere.generated.h"

UCLASS()
class  AMyProceduralSphere : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyProceduralSphere();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	float GridSize = 1000;
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel = 10;

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	bool Enable_Plain= false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	int32 NoiseSeed_Plain = 7777;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseHeight_Plain = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseFrequency_Plain = 0.01f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	FRuntimeVectorCurve NoiseDistribution_Plain;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void GenerateMesh();
	
	UFUNCTION()
	void GenerateSphereFace(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV0, TArray<FColor>& InVertexColor, FRotator Rot);
private:
	FVector GetSpherePosFromXY(int32 x, int32 y, FRotator Rot);
};