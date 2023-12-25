// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "MyProceduralSphere.generated.h"

UCLASS()
class MYCONTENTSHOW_API AMyProceduralSphere : public AActor
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	FRotator FaceRotation;
	
	// 网格大小
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	float GridSize = 1000;
	// X 轴顶点数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	int32 NoiseSeed = 7777;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
	float NoiseScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMin = "0.00001", UIMax = "0.1"), Category = "Noise")
	float NoiseFactor = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	UFUNCTION(BlueprintCallable)
	void GenerateMesh();
	
	UFUNCTION()
	void GenerateSphereFace(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals,
		TArray<FVector2D>& InUV0, TArray<FColor>& InVertexColor);
private:
	FVector GetSpherePosFromXY(int32 x, int32 y);
};