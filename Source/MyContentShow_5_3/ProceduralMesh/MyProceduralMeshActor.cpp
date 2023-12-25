// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProceduralMeshActor.h"
#include "SimplexNoiseBPLibrary.h"

// Sets default values
AMyProceduralMeshActor::AMyProceduralMeshActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(FName("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AMyProceduralMeshActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyProceduralMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMyProceduralMeshActor::GenerateMesh()
{
	if (GridSize.X <= 0 || GridSize.Y <= 0 || Sublevel_X < 1 || Sublevel_Y < 1) return;
	Mesh->ClearAllMeshSections();

	// Mesh buffers
	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FColor> vertexColors;

	TArray<float> heights;
	GetHeights(heights);

	GenerateGrid(vertices, triangles, normals, UV0, vertexColors, heights, GridSize, Sublevel_X, Sublevel_Y);
	Mesh->CreateMeshSection(0, vertices, triangles, normals, UV0, vertexColors, tangents, true);
	Mesh->SetMaterial(0, Material);
	//Mesh->CreateMeshSection_LinearColor(0, vertices, triangles, normals, UV0, vertexColors, tangents, true, false);
}

void AMyProceduralMeshActor::GenerateGrid(
	TArray<FVector>& InVertices,
	TArray<int32>& InTriangles,
	TArray<FVector>& InNormals, 
	TArray<FVector2D>& InUV0, 
	TArray<FColor>& InVertexColor,
	TArray<float> InNoiseHeight, 
	FVector2D InSize, 
	int32 InLength, 
	int32 InWidth)
{
	FVector2D SectionSize = FVector2D(InSize.X / InLength, InSize.Y / InWidth);
	int32 VertexIndex = 0;

	for (int X = 0; X < InLength + 1; X++)
	{
		for (int Y = 0; Y < InWidth + 1; Y++)
		{
			float z = VertexIndex < InNoiseHeight.Num() ? InNoiseHeight[VertexIndex] : 0;
			FVector PosPlane = FVector(X * SectionSize.X, Y * SectionSize.Y, z * NoiseScale);
			InVertices.Add(PosPlane);
			
			FLinearColor color = FLinearColor(1, 0, 0, z);
			InVertexColor.Add(color.ToFColor(true));
			// UV
			FVector2D uv = FVector2D((float)X / (float)InLength, (float)Y / (float)InWidth);
			InUV0.Add(uv);
			// Once we've created enough verts we can start adding polygons
			if (X > 0 && Y > 0)
			{
				int32 bTopRightIndex = (X * (InWidth + 1)) + Y;
				int32 bTopLeftIndex = bTopRightIndex - 1;
				int32 pBottomRightIndex = ((X - 1) * (InWidth + 1)) + Y;
				int32 pBottomLeftIndex = pBottomRightIndex - 1;

				// Now create two triangles from those four vertices
				// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
				InTriangles.Add(pBottomLeftIndex);
				InTriangles.Add(bTopRightIndex);
				InTriangles.Add(bTopLeftIndex);

				InTriangles.Add(pBottomLeftIndex);
				InTriangles.Add(pBottomRightIndex);
				InTriangles.Add(bTopRightIndex);
			}
			VertexIndex++;
		}
	}

	// normal
	for (int X = 0; X < InLength + 1; X++)
	{
		for (int Y = 0; Y < InWidth + 1; Y++)
		{
			int32 c = (X * (InWidth + 1)) + Y;
			int32 centerUp = c + InWidth + 1;
			int32 centerBottom = c - (InWidth + 1);
			int32 centerRight = c + 1;
			if (centerRight > ((X * (InWidth + 1)) + InWidth))
			{
				centerRight = -1;
			}
			int32 centerLeft = c - 1;
			if (centerLeft < (X * (InWidth + 1)))
			{
				centerLeft = -1;
			}
			int32 centerUpRight = centerUp + 1;
			if (centerUpRight > (X + 1) * (InWidth + 1) + InWidth)
			{
				centerUpRight = -1;
			}
			int32 centerBottomLeft = centerBottom - 1;
			if (centerBottomLeft < (X - 1) * (InWidth + 1))
			{
				centerBottomLeft = -1;
			}

			int32 indexs[6] = { centerUp,centerUpRight,centerRight,centerBottom,centerBottomLeft,centerLeft };
			TArray<FVector> vers;
			// get all the normal of triangles that using the current vertex
			for (int i = 0; i < 6; i++)
			{
				int32 NextIndex = (i + 1 >= 6) ? 0 : (i + 1);
				if (indexs[i] >= 0 && indexs[i] < InVertices.Num() && indexs[NextIndex] >= 0 && indexs[NextIndex] < InVertices.Num())
				{
					FVector a = (InVertices[indexs[i]] - InVertices[c]).GetUnsafeNormal();
					FVector b = (InVertices[indexs[NextIndex]] - InVertices[c]).GetUnsafeNormal();
					vers.Add(FVector::CrossProduct(a, b).GetUnsafeNormal());
					break;
				}
			}

			// get the average Vector
			FVector NormalSum = FVector(0, 0, 1);
			for (auto& i : vers)
			{
				NormalSum += i;
			}
			InNormals.Add(NormalSum / vers.Num());
		}
	}
}

void AMyProceduralMeshActor::GetHeights(TArray<float>& InHeights)
{
	if (NoiseScale == 0) return;
	USimplexNoiseBPLibrary::setNoiseSeed(NoiseSeed);
	for (auto X = 0; X < Sublevel_X + 1; X++)
	{
		for (auto Y = 0; Y < Sublevel_Y + 1; Y++)
		{
			InHeights.Add(USimplexNoiseBPLibrary::SimplexNoise2D(X, Y, NoiseFactor));
		}
	}
}
