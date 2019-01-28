// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BountyDashObject.h"
#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "BountyDashPowerUp.generated.h"
/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ABountyDashPowerUp : public ABountyDashObject
{
	GENERATED_BODY()
public :
	ABountyDashPowerUp();
	
	virtual void MyOnActorOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
protected:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;
private:
	class UPowerUpObject* PowerUp;
};
