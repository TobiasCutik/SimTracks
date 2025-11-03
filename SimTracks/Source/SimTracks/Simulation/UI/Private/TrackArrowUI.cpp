// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/TrackArrowUI.h"

void UTrackArrowUI::SetArrowLineHeight(float Height)
{
	FVector2D ImageSize = ArrowLine->GetBrush().ImageSize;
	ImageSize.Y = Height;

	ArrowLine->SetDesiredSizeOverride(ImageSize);
}

FVector2D UTrackArrowUI::GetArrowHeadImageSize() const
{
	return ArrowHead->GetBrush().ImageSize;
}