// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSHud.h"

#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"

AFPSHud::AFPSHud()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	crosshair = CrosshairObj.Object;
}


void AFPSHud::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, crosshair->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

//	const UFont* font;
	FText text;

	//need to find a way to find the player, then reference the weapon used by them
	int clip = 0;
	int reserve = 0;

	//FCanvasTextItem AmmoText(FVector2D::ZeroVector, text, font, FLinearColor::Red);
	//AmmoText.Text = FText::Format(LOCTEXT("AmmoDisplayText", "{0} / {1}"), clip, reserve, FName("AmmoText")); //this doesnt work for some reason
	//Canvas->DrawItem(AmmoText, 10, 10);
}