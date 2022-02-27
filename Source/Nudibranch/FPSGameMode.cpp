// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"

#include "Blueprint/UserWidget.h"
#include "kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	CurrLvlIndex = 0;
}

void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();

	Controller = GetWorld()->GetFirstPlayerController();

	FInputModeGameOnly inMode;
	Controller->SetInputMode(inMode);
	Controller->bShowMouseCursor = false;

	CheckLevel();
}

void AFPSGameMode::CheckLevel()
{
	FString CurrLvlName = GetWorld()->GetMapName();

	LvlNames.Find(CurrLvlName, CurrLvlIndex);

	if (CurrLvlIndex < LvlNames.Num() - 1)
	{
		NextLvl = LvlNames[CurrLvlIndex + 1];
	}
	else
	{
		NextLvl = "GameEnd";
	}
}

void AFPSGameMode::ResetGame()
{
	FString LvlString = GetWorld()->GetMapName();
	FName LvlToLoad = FName(*LvlString);

	UGameplayStatics::OpenLevel(this, LvlToLoad, true);
}

void AFPSGameMode::LevelComplete()
{
	if (DefLvlCompleteWidget)
	{
		LvlCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefLvlCompleteWidget);
		LvlCompleteWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No deflvl widget"));
	}

	GetWorldTimerManager().SetTimer(TimerHandle_LvlSwapTimer, this, &AFPSGameMode::LoadNextLevel, 2.0f, false);
}

void AFPSGameMode::LoadNextLevel()
{
	if (LvlNames.Contains(NextLvl))
	{
		FName LvlToLoad = FName(*NextLvl);
		UGameplayStatics::OpenLevel(this, LvlToLoad, true);
	}
	else
	{
		//show end game widget

		if (LvlCompleteWidget)
		{
			LvlCompleteWidget->RemoveFromParent();

			if (DefLvlCompleteWidget)
			{
				GameCompleteWidget = CreateWidget<UUserWidget>(GetWorld(), DefGameCompleteWidget);
				GameCompleteWidget->AddToViewport();

				Controller->bShowMouseCursor = true;
				FInputModeUIOnly inMode;
				Controller->SetInputMode(inMode);
			}
		}
	}
}
