// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashGameMode.h"
#include "BountyDashChararcter.h"
#include "BountyDashHUD.h"

ABountyDashGameMode::ABountyDashGameMode()
{
	DefaultPawnClass = ABountyDashChararcter::StaticClass();

	numCoinsForSpeedIncrease = 5;
	gameSpeed = 30.0f;
	gameSpeedIncrease = 5.0f;
	gameLevel = 1;
	RunTime = 0.0f;
	bGameOver = false;
	startGameOverCount = false;
	timeTillGameOver = 2.0f;
	gameOverTimer = 0.0f;

	HUDClass = ABountyDashHUD::StaticClass();

	
}

// Called every frame
void ABountyDashGameMode::Tick(float DeltaTime)
{
	if(!startGameOverCount)
	{
		RunTime += DeltaTime;
	}
	else
	{
		gameOverTimer += DeltaTime;
		if (gameOverTimer >= timeTillGameOver)
		{
			bGameOver = true;
		}
	}
	
}
void ABountyDashGameMode::CharScoreUp(unsigned int charScore)
{
	if (charScore != 0 && charScore%numCoinsForSpeedIncrease == 0)
	{
		gameSpeed += gameSpeedIncrease;
		gameLevel++;
	}
}

float ABountyDashGameMode::GetInvGameSpeed()
{
	return -gameSpeed;
}

float ABountyDashGameMode::GetGameSpeed()
{
	return gameSpeed;
}

int32 ABountyDashGameMode::GetGameLevel()
{
	return gameLevel;
}

void ABountyDashGameMode::ReduceGameSpeed()
{
	if (gameSpeed > 10.0f)
	{
		gameSpeed -= gameSpeedIncrease;
		gameLevel--;
	}
}

float ABountyDashGameMode::GetRunTime()
{
	return RunTime;
}

bool ABountyDashGameMode::GetGameOver()
{
	return bGameOver;
}

void ABountyDashGameMode::GameOver()
{
	startGameOverCount = true;
}

void ABountyDashGameMode::SetGamePaused(bool gamePaused)
{
	APlayerController* myPlayer = GetWorld()->GetFirstPlayerController();
	if (myPlayer != nullptr)
	{
		myPlayer->SetPause(gamePaused);
	}
}