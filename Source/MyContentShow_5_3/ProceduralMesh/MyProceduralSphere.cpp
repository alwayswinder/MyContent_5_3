// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProceduralSphere.h"
#include "SimplexNoiseBPLibrary.h"

// Sets default values
AMyProceduralSphere::AMyProceduralSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(FName("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AMyProceduralSphere::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyProceduralSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMyProceduralSphere::GenerateMesh()
{
	if (GridSize <= 0 || Sublevel < 1) return;
	Mesh->ClearAllMeshSections();

	TArray<FRotator> Rotators = {FRotator(0, 0, 0),
								FRotator(0, 0, 90),
								FRotator(0, 0, -90),
								FRotator(90, 0, 0),
								FRotator(-90, 0, 0),
								FRotator(0, 0, 180)};
	for (int i=0; i<6; i++)
	{
		// Mesh buffers
		TArray<FVector> vertices;
		TArray<int32> triangles;
		TArray<FVector> normals;
		TArray<FVector2D> UV0;
		TArray<FProcMeshTangent> tangents;
		TArray<FColor> vertexColors;
		
		GenerateSphereFace(vertices, triangles, normals, UV0, vertexColors, Rotators[i]);
		Mesh->CreateMeshSection(i, vertices, triangles, normals, UV0, vertexColors, tangents, true);
		Mesh->SetMaterial(i, Material);
	}
}

void AMyProceduralSphere::GenerateSphereFace(
	TArray<FVector>& InVertices,
	TArray<int32>& InTriangles,
	TArray<FVector>& InNormals, 
	TArray<FVector2D>& InUV0, 
	TArray<FColor>& InVertexColor,
	FRotator Rot)
{
	int32 VertexIndex = 0;
	TMap<FVector2d, FVector> PosSave;
	for (int X = -1; X < Sublevel + 2; X++)
	{
		for (int Y = -1; Y < Sublevel + 2; Y++)
		{
			PosSave.Add(FVector2d(X, Y), GetSpherePosFromXY(X, Y, Rot));
		}
	}
	
	for (int X = 0; X < Sublevel + 1; X++)
	{
		for (int Y = 0; Y < Sublevel + 1; Y++)
		{
			FVector PosSphere = PosSave[FVector2d(X, Y)];
			InVertices.Add(PosSphere);
			
			FLinearColor color = FLinearColor(1, 0, 0, PosSphere.Z);
			InVertexColor.Add(color.ToFColor(true));
			// UV
			FVector2D uv = FVector2D((float)X / (float)Sublevel, (float)Y / (float)Sublevel);
			InUV0.Add(uv);
			// Once we've created enough verts we can start adding polygons
			if (X > 0 && Y > 0)
			{
				int32 bTopRightIndex = (X * (Sublevel + 1)) + Y;
				int32 bTopLeftIndex = bTopRightIndex - 1;
				int32 pBottomRightIndex = ((X - 1) * (Sublevel + 1)) + Y;
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
	for (int X = 0; X < Sublevel + 1; X++)
	{
		for (int Y = 0; Y < Sublevel + 1; Y++)
		{
			int32 c = (X * (Sublevel + 1)) + Y;
			
			TArray<FVector> Posneighbors;
			Posneighbors.Add(PosSave[FVector2d(X+1, Y)]);
			Posneighbors.Add(PosSave[FVector2d(X, Y+1)]);
			Posneighbors.Add(PosSave[FVector2d(X-1, Y)]);
			Posneighbors.Add(PosSave[FVector2d(X, Y-1)]);


			TArray<FVector> vers;
			// get all the normal of triangles that using the current vertex
			for (int i = 0; i < 4; i++)
			{
				int32 NextIndex = (i + 1 >= 4) ? 0 : (i + 1);
				FVector a = (Posneighbors[i] - InVertices[c]).GetUnsafeNormal();
				FVector b = (Posneighbors[NextIndex] - InVertices[c]).GetUnsafeNormal();
				vers.Add(FVector::CrossProduct(a, b).GetUnsafeNormal());
			}

			// get the average Vector
			FVector NormalSum = FVector(0, 0, 1);
			for (auto& i : vers)
			{
				NormalSum += i;
			}
			FVector NormalResult = NormalSum / vers.Num();
			NormalResult.Normalize();
			InNormals.Add(NormalResult);
		}
	}
}

FVector AMyProceduralSphere::GetSpherePosFromXY(int32 X, int32 Y, FRotator Rot)
{
	//归一化平面坐标
	FVector PosPlane = FVector((X*1.f)/Sublevel*2.f-1.f, (Y*1.f)/Sublevel*2.f-1.f, 1.f);
	
	//转换到球面
	float x2 = PosPlane.X * PosPlane.X;
	float y2 = PosPlane.Y * PosPlane.Y;
	float z2 = PosPlane.Z * PosPlane.Z;
	float xp = PosPlane.X * sqrt(1-(y2+z2)/2 + (y2*z2)/3);
	float yp = PosPlane.Y * sqrt(1-(z2+x2)/2 + (z2*x2)/3);
	float zp = PosPlane.Z * sqrt(1-(x2+y2)/2 + (x2*y2)/3);
	FVector PosSphere = FVector(xp, yp, zp);
	
	//旋转
	PosSphere = Rot.RotateVector(PosSphere);
	FVector Normal = PosSphere;
	Normal.Normalize();
	float Height = 0.f;
	
	if(Enable_Mountain)
	{
		USimplexNoiseBPLibrary::setNoiseSeed(NoiseSeed_Mountain);
		float PerlinValue = USimplexNoiseBPLibrary::SimplexNoise3D(PosSphere.X, PosSphere.Y, PosSphere.Z,
			NoiseFrequency_Mountain) + 1;
		//UE_LOG(LogTemp,Warning,TEXT("pos = %f, %f, %f"),PosSphere.X, PosSphere.Y, PosSphere.Z);
		float NoiseDis_X =  NoiseDistribution_Mountain.GetValue(PosSphere.X).X;
		float NoiseDis_Y =  NoiseDistribution_Mountain.GetValue(PosSphere.Y).Y;
		float NoiseDis_Z =  NoiseDistribution_Mountain.GetValue(PosSphere.Z).Z;
		
		Height += PerlinValue*NoiseHeight_Mountain*NoiseDis_X*NoiseDis_Y*NoiseDis_Z;
	}
	
	return PosSphere*GridSize*0.5f + Height*Normal;
}
