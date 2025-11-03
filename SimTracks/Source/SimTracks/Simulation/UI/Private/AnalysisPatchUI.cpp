// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/AnalysisPatchUI.h"

bool UAnalysisPatchUI::WasCreatedAtTime(float Time) const
{
	return FMath::IsNearlyEqual(SimulationTime, Time, KINDA_SMALL_NUMBER);
}

void UAnalysisPatchUI::SetDesiredSize(FVector2D Size)
{
	Patch->SetDesiredSizeOverride(Size);
}