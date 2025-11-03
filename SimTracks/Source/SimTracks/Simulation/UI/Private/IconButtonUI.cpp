// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/IconButtonUI.h"
#include "Components/Image.h"

void UIconButtonUI::SwitchActiveTexture()
{
	if (bAlternativeTextureActive)
		TextureTarget->SetBrushFromTexture(MainTexture);
	else
		TextureTarget->SetBrushFromTexture(AlternativeTexture);

	bAlternativeTextureActive = !bAlternativeTextureActive;
}