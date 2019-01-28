// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashHUD.h"
#include "BountyDash.h"
#include "BountyDashChararcter.h"
#include "BountyDashGameMode.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/Canvas.h"

ABountyDashHUD::ABountyDashHUD()
{
	static ConstructorHelpers::FObjectFinder<UFont> OurHudFont(TEXT("/Game/BountyDashFont.BountyDashFont"));
	HUDFont = OurHudFont.Object;
}

void ABountyDashHUD::DrawHUD()
{
	Super::DrawHUD();
	
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	ABountyDashChararcter* DashCharacter = Cast<ABountyDashChararcter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	ABountyDashGameMode* DashGameMode = GetCustomGameMode<ABountyDashGameMode>(GetWorld());

	FString HUDString = FString::Printf(TEXT("Score: %d Runtime: %.4f"), DashCharacter->GetScore(), DashGameMode->GetRunTime());

	DrawText(HUDString, FColor::Yellow, 50, 50, HUDFont);

	if (DashGameMode->GetGameOver())
	{
		FVector2D GameOverSize;
		GetTextSize(TEXT("GAME OVER!! Press R to Restart!"), GameOverSize.X, GameOverSize.Y, HUDFont);

		DrawText(TEXT("GAME OVER!! Press R to Restart!"), FColor::Yellow, (ScreenDimensions.X - GameOverSize.X) / 2.0f, (ScreenDimensions.Y - GameOverSize.Y) / 2.0f, HUDFont);

		DashGameMode->SetGamePaused(true);
	}
}