// Copyright (c) 2025 Tobias Cutik


#include "SimTracks/Simulation/UI/Public/HistoryChangeUI.h"
#include "SimTracks/Simulation/Utils/Public/TextUtils.h"

void UHistoryChangeUI::Init(const FString& ChangedSimParamName, float ChangedSimParamValue, const FString& TrackStartTime)
{
	SimulationParam->SetText(FText::FromString(ChangedSimParamName));
	ChangedValue->SetText(UTextUtils::AsEnNumber(ChangedSimParamValue));
	BranchingTime->SetText(FText::FromString(TrackStartTime));
}
