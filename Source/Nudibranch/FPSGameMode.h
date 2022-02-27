// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NUDIBRANCH_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	AFPSGameMode();
	void ResetGame();
	void LevelComplete();
	void LoadNextLevel();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS Game Mode")
		TSubclassOf<UUserWidget> DefLvlCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS Game Mode")
		UUserWidget* LvlCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS Game Mode")
		TSubclassOf<UUserWidget> DefGameCompleteWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FPS Game Mode")
		UUserWidget* GameCompleteWidget;

private:

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category = "FPS Game Mode")
		TArray<FString> LvlNames;

	APlayerController* Controller;

	int32 CurrLvlIndex;
	FString NextLvl;

	FTimerHandle TimerHandle_LvlSwapTimer;

	void CheckLevel();
};
