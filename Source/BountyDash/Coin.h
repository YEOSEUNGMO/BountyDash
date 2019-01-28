// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDashObject.h"
#include "CoreMinimal.h"
#include "Obstacle.h"
#include "Coin.generated.h"

/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ACoin : public ABountyDashObject
{
	GENERATED_BODY()

		ACoin();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool BeingPulled;
protected:
		virtual void MyOnActorOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
		UStaticMeshComponent* Mesh;
};
