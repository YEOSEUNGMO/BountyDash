// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "PowerUpObject.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BountyDashChararcter.generated.h"

UCLASS()
class BOUNTYDASH_API ABountyDashChararcter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABountyDashChararcter();

	UPROPERTY(EditAnywhere, Category = Logic)
		TArray<class ATargetPoint*> TargetArray;

	UPROPERTY(EditAnywhere, Category = Logic)
		float CharSpeed;

	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* hitObstacleSound;

	UPROPERTY(EditAnywhere, Category = Sound)
		UAudioComponent* dingSound;

	UFUNCTION()
		void StopSmash();
	UFUNCTION()
		void StopMagnet();

	void ScoreUp();
	void PowerUp(EPowerUp Type);
	int GetScore();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly)
		int32 Score;

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float SmashTime;


	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagnetTime;

	UPROPERTY(EditAnywhere, Category = PowerUps)
		float MagetReach;

	UFUNCTION()
		void MyOnComponentOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void MyOnComponentEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void Reset();

	void MoveRight();
	void MoveLeft();
	void CoinMagnet();

	float KillPoint;
	
private:
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;
	bool CanSmash;
	bool CanMagnet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
