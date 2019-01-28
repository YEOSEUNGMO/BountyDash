// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

UCLASS()
class BOUNTYDASH_API AFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloor();

	float GetKillPoint();
	float GetSpawnPoint();

protected:
	UPROPERTY(EditAnywhere)
		TArray<USceneComponent*> FloorMeshScenes;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMeshComponent*>FloorMeshes;

	UPROPERTY(EditAnywhere)
		UBoxComponent* CollisionBox;
	
	int32 NumRepeatingMesh;
	float KillPoint;
	float SpawnPoint;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
