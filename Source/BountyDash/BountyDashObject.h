// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BountyDashObject.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashObject : public AActor
{
	GENERATED_BODY()

		float KillPoint;

public:	
	// Sets default values for this actor's properties
	ABountyDashObject();

	void SetKillPoint(float point);
	float GetKillPoint();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void MyOnActorOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void MyOnActorEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)
		USphereComponent* Collider;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
