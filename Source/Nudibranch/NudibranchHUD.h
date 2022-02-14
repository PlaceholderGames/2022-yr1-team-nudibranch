// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NudibranchHUD.generated.h"

UCLASS()
class ANudibranchHUD : public AHUD
{
	GENERATED_BODY()

public:
	ANudibranchHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

