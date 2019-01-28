// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BountyDashGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BOUNTYDASH_API ABountyDashGameMode : public AGameMode
{
	GENERATED_BODY()

	UPROPERTY()
	float gameSpeed;

	UPROPERTY()
	int32 gameLevel;

public:
	ABountyDashGameMode();

	// Called every frame
	UFUNCTION()
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		float GetInvGameSpeed();

	UFUNCTION()
		float GetGameSpeed();

	UFUNCTION()
		int32 GetGameLevel();

	UFUNCTION()
		bool GetGameOver();

	UFUNCTION()
		void GameOver();

	UFUNCTION()
		void SetGamePaused(bool gamePaused);
	void CharScoreUp(unsigned int charScore);
	void ReduceGameSpeed();
	float GetRunTime();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 numCoinsForSpeedIncrease;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float gameSpeedIncrease;

	UPROPERTY()
		float RunTime;

	UPROPERTY()
		bool bGameOver;

	UPROPERTY()
		bool startGameOverCount;
	
	UPROPERTY()
		float timeTillGameOver;

	UPROPERTY()
		float gameOverTimer;
};
