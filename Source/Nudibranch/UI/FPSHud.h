// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FPSHud.generated.h"

/**
 * 
 */
UCLASS()
class NUDIBRANCH_API AFPSHud : public AHUD
{
	GENERATED_BODY()
	
public:
	AFPSHud();
	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* crosshair;
};
