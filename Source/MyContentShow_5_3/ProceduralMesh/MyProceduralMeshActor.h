// Fill out your copyright notice in the Description page of Project Settings.
#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
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

	// �����С
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	FVector2D GridSize = FVector2D(1000, 1000);
	// X �ᶥ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel_X = 10;
	// Y �ᶥ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid Generate")
	int32 Sublevel_Y = 10;

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
	void GenerateGrid(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FVector2D>& InUV0, TArray<FColor>& InVertexColor, TArray<float>InNoiseHeight, FVector2D InSize, int32 InLength, int32 InWidth);
private:
	void GetHeights(TArray<float>& InHeights);
};